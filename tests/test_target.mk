this_dirs := $(subst /, ,$(shell (cd $(d); pwd;)))
this_test := $(word $(words $(this_dirs)),$(this_dirs))

define this_rule
test:: $(prorab_this_name)
	@prorab-running-test.sh $(this_test)
	@(cd $(d); ./tests); if [ $$$$? -ne 0 ]; then prorab-error.sh "test failed"; else prorab-passed.sh; fi
endef
$(eval $(this_rule))
