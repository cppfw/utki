cmake_minimum_required(VERSION 3.10)

project(utki)

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${PROJECT_NAME}
    RECURSIVE
)

myci_declare_library(${PROJECT_NAME}
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    PRIVATE_INCLUDE_DIRECTORIES
        ../../src_deps
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${PROJECT_NAME}
)

# if the library is compiled by vcpkg during the port build,
# then we don't need to build unit tests
if(NOT IS_VCPKG_PORT_BUILD)
    include(unit_tests.cmake)
endif()
