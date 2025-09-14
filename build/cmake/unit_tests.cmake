set(test_srcs)
myci_add_source_files(test_srcs
    DIRECTORY
        ../../tests/unit/src
    RECURSIVE
)

myci_add_source_files(test_srcs
    DIRECTORY
        ../../tests/harness/tst/src
    RECURSIVE
)

myci_add_source_files(test_srcs
    DIRECTORY
        ../../tests/harness/clargs/src
    RECURSIVE
)

myci_declare_application(${name}-tests
    GUI
    SOURCES
        ${test_srcs}
    PREPROCESSOR_DEFINITIONS
        TST_NO_PAR
    DEPENDENCIES
        utki
)

if(NOT TARGET test)
    add_custom_target(test)
endif()

add_custom_command(TARGET test
    POST_BUILD
    COMMAND
        $<TARGET_FILE:${name}-tests>
)
