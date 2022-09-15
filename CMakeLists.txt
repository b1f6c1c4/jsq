cmake_minimum_required(VERSION 3.16)
project(jsq)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

find_package(Boost 1.79.0 REQUIRED)

add_compile_options(-Wall -Wextra -Werror)
add_compile_options(-Wno-parentheses)
add_compile_options(-Wno-unknown-pragmas)
add_compile_options(-Wno-missing-field-initializers)
add_compile_options(-Wno-unused-but-set-variable)
add_compile_options(-Wno-unused-function)
add_compile_options(-Wno-unused-label)
add_compile_options(-Wno-unused-local-typedefs)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-sign-compare)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include_directories(include)

add_executable(jsq main.cpp)
