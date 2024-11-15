cmake_minimum_required(VERSION 3.10)

set(name utki)

project(${name})

file(GLOB_RECURSE srcs "../src/${name}/*.cpp")
file(GLOB_RECURSE hdrs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../src" "../src/${name}/*.hpp")

# message(hdrs=${hdrs})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(
        ${name}
        STATIC
        ${srcs}
    )

# target_sources(
#     ${name}
#     PUBLIC
#     ${hdrs}
# )

target_include_directories(
        ${name}
        PRIVATE
        "../src_deps"
    )

# target_link_libraries(
#         ${name}
#     )

install(TARGETS ${name})
