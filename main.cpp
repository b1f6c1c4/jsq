#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/tuple/tuple.hpp>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

enum class operation_t {
    PUSH,
    D,
    CD,
    ABCD,
};

enum edge_op_t { edge_op };

namespace boost {
    BOOST_INSTALL_PROPERTY(edge, op);
};

int main() {
    using namespace boost;
    typedef property<edge_op_t, operation_t> op_t;
    typedef property<edge_weight_t, unsigned, op_t> weight_t;
    typedef adjacency_list<vecS, vecS, bidirectionalS, no_property, weight_t> graph_t;
    typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;

    std::mutex mtx{};
    graph_t g{ 10000 };

    for (auto num = 0; num < 10000; num++) {
        const auto a = num / 1000, b = num / 100 % 10, c = num / 10 % 10, d = num % 10;

        auto n_answers = 0zu;
        struct {
            operation_t op;
            unsigned weight;
            int n;
        } answers[8];

        answers[n_answers++] = { operation_t::PUSH, 1, (num + 1) % 10000 };

        unsigned w_cd = 3;
        if (a == c) w_cd += 2;
        if (b == c) w_cd += 2;

        if (c == d) {
            if (!((a == 9 || b == 9) && c == 9))
                answers[n_answers++] = { operation_t::D, 8, (num - d) + (d + 1) % 10 };
            answers[n_answers++] = { operation_t::CD, w_cd, (num - c * 11) + (c + 1) % 10 * 11 };
        } else {
            answers[n_answers++] = { operation_t::D, 3, (num - d) + (d + 1) % 10 };
            answers[n_answers++] = { operation_t::CD, w_cd, (num - c * 10) + (c + 1) % 10 * 10 };
        }

        int x[] = { a, b, c, d };
        if (x[0] > x[2]) std::swap(x[0], x[2]);
        if (x[1] > x[3]) std::swap(x[1], x[3]);
        if (x[0] > x[1]) std::swap(x[0], x[1]);
        if (x[2] > x[3]) std::swap(x[2], x[3]);
        if (x[1] > x[2]) std::swap(x[1], x[2]);

        for (auto j = 0zu; j < 4zu; j++) {
            int k = 0;
            if (a == x[j]) k += 1000;
            if (b == x[j]) k += 100;
            if (c == x[j]) k += 10;
            if (d == x[j]) k += 1;
            answers[n_answers++] = { operation_t::ABCD, 4, (num - x[j] * k) + (x[j] + 1) % 10 * k };
        }

        std::lock_guard lock{ mtx };
        for (auto j = 0zu; j < n_answers; j++)
            add_edge(num, answers[j].n, weight_t{ answers[j].weight, op_t{ answers[j].op } }, g);
    }

    std::cerr << "Number of Vertices: " << num_vertices(g) << std::endl;
    std::cerr << "Number of Edges: " << num_vertices(g) << std::endl;

    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<unsigned> d(num_vertices(g));
    vertex_descriptor s = vertex(0, g);
    dijkstra_shortest_paths(g, s,
            predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, g)))
            .distance_map(make_iterator_property_map(d.begin(), get(vertex_index, g))));

    int max = 0;
    unsigned max_d = 0;
    for (auto i = 0zu; i < num_vertices(g); i++)
        if (d[i] > max_d)
            max_d = d[i], max = i;

    std::cerr << "The hardest number is: " << max << " (" << max_d << ")" << std::endl;

    while (true) {
        std::cerr << "What number do you want?" << std::endl;
        int num = -1;
        std::cin >> num;
        if (num < 0)
            break;
        for (; num; num = p[num])
            std::cout << std::setfill('0') << std::setw(4) << p[num] << std::endl;
    }
}
