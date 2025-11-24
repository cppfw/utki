# if the library is compiled by vcpkg during the port build (i.e. during the package installation),
# then we don't need to build unit tests
if(IS_VCPKG_PORT_BUILD)
    return()
endif()

# no unit tests for ios
if(IOS)
    return()
endif()

# For unit tests we need the tst testing framework library which in turn depends on utki.
if(CPPFW_MONOREPO)
    # In case of monorepo build we can add necessary tst dependencies right here.

    myci_add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../clargs/build/cmake
        BINARY_DIR
            clargs
    )
    myci_add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../opros/build/cmake
        BINARY_DIR
            opros
    )
    myci_add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../nitki/build/cmake
        BINARY_DIR
            nitki
    )
    myci_add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../tst/build/cmake
        BINARY_DIR
            tst
    )

    set(tst_dep tst::tst)
else()
    # In case of non-monorepo build, we need to build the tst framework
    # from sources (from git submodules) together with unit tests.

    set(utki_test_harness_dir ${CMAKE_CURRENT_LIST_DIR}/../../tests/harness)

    myci_add_source_files(tst_srcs
        DIRECTORY
            ${utki_test_harness_dir}/tst/src
        RECURSIVE
    )

    myci_add_source_files(tst_srcs
        DIRECTORY
            ${utki_test_harness_dir}/clargs/src
        RECURSIVE
    )

    myci_declare_library(tst
        SOURCES
            ${tst_srcs}
        PUBLIC_INCLUDE_DIRECTORIES
            ${utki_test_harness_dir}/tst/src
        PRIVATE_INCLUDE_DIRECTORIES
            ${utki_test_harness_dir}/clargs/src
        PREPROCESSOR_DEFINITIONS
            TST_NO_PAR
        DEPENDENCIES
            utki
    )

    set(tst_dep ${PROJECT_NAME}::tst)
endif()

set(test_srcs)
myci_add_source_files(test_srcs
    DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/../../tests/unit/src
    RECURSIVE
)

myci_declare_application(${PROJECT_NAME}-tests
    GUI
    SOURCES
        ${test_srcs}
    DEPENDENCIES
        utki
        ${tst_dep}
)

myci_declare_test(${PROJECT_NAME}-tests)
