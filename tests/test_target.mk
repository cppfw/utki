this_dirs := $(subst /, ,$(shell (cd $(d); pwd;)))
this_test := $(word $(words $(this_dirs)),$(this_dirs))

define this_rule
test:: $(prorab_this_name)
	@myci-running-test.sh $(this_test)
	@(cd $(d); LD_LIBRARY_PATH=../../src/build ./tests); if [ $$$$? -ne 0 ]; then myci-error.sh "test failed"; else myci-passed.sh; fi
endef
$(eval $(this_rule))
