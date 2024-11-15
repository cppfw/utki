cmake_minimum_required(VERSION 3.10)

set(name utki)

file(GLOB_RECURSE srcs "../src/${name}/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(
        ${name}
        STATIC
        ${srcs}
    )

target_include_directories(
        ${name}
        PRIVATE
        "../src_deps"
    )

# target_link_libraries(
#         ${name}
#     )
