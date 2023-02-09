include $(config_dir)rel.mk

this_lint_cmd = $(prorab_lint_cmd_clang_tidy)

ifeq ($(os),macosx)
    # WORKAROUND:
    # clang-tidy on macos doesn't use /usr/local/include as default place to
    # search for header files, so we add it explicitly
    this_cxxflags += -I /usr/local/include
endif
