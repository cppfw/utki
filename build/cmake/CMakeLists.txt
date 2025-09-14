cmake_minimum_required(VERSION 3.10)

set(name utki)
project(${name})

# !!! find_package must go after project() declaration !!!
# Otherwise VCPKG does not set the CMAKE_PREFIX_PATH to find packages.
find_package(myci CONFIG REQUIRED)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${name}
    RECURSIVE
)

myci_declare_library(${name}
    SOURCES
        ${srcs}
    PUBLIC_INCLUDE_DIRECTORIES
        ../../src
    PRIVATE_INCLUDE_DIRECTORIES
        ../../src_deps
    INSTALL_INCLUDE_DIRECTORIES
        ../../src/${name}
)

# if the library is compiled by vcpkg during the library package installation,
# then we don't need to build unit tests
if(NOT DEFINED VCPKG_INSTALL_PREFIX)
    include(unit_tests.cmake)
endif()
