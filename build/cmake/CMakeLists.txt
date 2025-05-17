cmake_minimum_required(VERSION 3.10)

set(name utki)

project(${name})

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

find_package(myci CONFIG REQUIRED)

set(srcs)
myci_add_source_files(srcs
    DIRECTORY
        ../../src/${name}
    PATTERNS
        *.cpp
        *.hpp
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
