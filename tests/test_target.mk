ifeq ($(prorab_os),windows)
    this_test_cmd := (cd $(prorab_this_dir); cp ../../src/libutki.dll . || true; ./$$(notdir $$^))
else
    ifeq ($(prorab_os),macosx)
        this_test_cmd := (cd $(prorab_this_dir); DYLD_LIBRARY_PATH=../../src ./$$(notdir $$^))
    else
        this_test_cmd := (cd $(prorab_this_dir); LD_LIBRARY_PATH=../../src ./$$(notdir $$^))
    endif
endif

define this_rule
test:: $(prorab_this_name)
	@echo running $$^...
	@$(this_test_cmd)
endef
$(eval $(this_rule))






ifeq ($(prorab_os),windows)
    this_gdb_cmd := (cd $(prorab_this_dir); cp ../../src/libutki.dll . || true; gdb ./$$(notdir $$^))
else
    ifeq ($(prorab_os),macosx)
        this_gdb_cmd := (cd $(prorab_this_dir); DYLD_LIBRARY_PATH=../../src gdb ./$$(notdir $$^))
    else
        this_gdb_cmd := (cd $(prorab_this_dir); LD_LIBRARY_PATH=../../src gdb ./$$(notdir $$^))
    endif
endif


define this_rule
gdb:: $(prorab_this_name)
	@echo running $$^...
	@$(this_gdb_cmd)
endef
$(eval $(this_rule))
