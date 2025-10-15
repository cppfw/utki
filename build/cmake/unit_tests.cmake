# if the library is compiled by vcpkg during the port build,
# then we don't need to build unit tests
if(IS_VCPKG_PORT_BUILD)
    return()
endif()

# no unit tests for ios
if(IOS)
    return()
endif()

set(test_srcs)
myci_add_source_files(test_srcs
    DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/../../tests/unit/src
    RECURSIVE
)

if(CPPFW_MONOREPO)
    set(utki_test_harness_dir ${CMAKE_CURRENT_LIST_DIR}/../../..)
else()
    set(utki_test_harness_dir ${CMAKE_CURRENT_LIST_DIR}/../../tests/harness)
endif()

myci_add_source_files(test_srcs
    DIRECTORY
        ${utki_test_harness_dir}/tst/src
    RECURSIVE
)

myci_add_source_files(test_srcs
    DIRECTORY
        ${utki_test_harness_dir}/clargs/src
    RECURSIVE
)

myci_declare_application(${PROJECT_NAME}-tests
    GUI
    SOURCES
        ${test_srcs}
    INCLUDE_DIRECTORIES
        ${utki_test_harness_dir}/tst/src
        ${utki_test_harness_dir}/clargs/src
    PREPROCESSOR_DEFINITIONS
        TST_NO_PAR
    DEPENDENCIES
        utki
)

if(NOT TARGET test)
    add_custom_target(test)
endif()

add_custom_target(test-${PROJECT_NAME})
add_dependencies(test test-${PROJECT_NAME})

add_custom_command(TARGET test-${PROJECT_NAME}
    POST_BUILD
    COMMAND
        $<TARGET_FILE:${PROJECT_NAME}-tests>
)
