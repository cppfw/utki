cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

set(name utki)

project(${name})

file(GLOB_RECURSE srcs "../src/${name}/*.cpp")

add_library(
    ${name}
    STATIC
    ${srcs}
)

target_compile_features(${name} PUBLIC cxx_std_17)

target_include_directories(
    ${name}
    PRIVATE "../src_deps"
)

install(TARGETS ${name})

# install library header files preserving directory hierarchy
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../src/${name}"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.hpp"
)
