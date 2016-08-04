define this_rule
test:: $(prorab_this_name)
	@echo running $$^...
	@$(this_test_cmd)
endef
$(eval $(this_rule))




define this_rule
gdb:: $(prorab_this_name)
	@echo running $$^...
	@$(this_gdb_cmd)
endef
$(eval $(this_rule))
