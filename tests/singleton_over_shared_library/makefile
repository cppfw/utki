include prorab.mk
include prorab-test.mk

this_name := testso
this_soname := 0

$(eval $(call prorab-config, ../../config))

this_srcs += testso.cpp

this_no_install := true

this_ldflags += -L $(d)../../src/out/$(c)
this_ldlibs += -lutki

$(eval $(prorab-build-lib))

lib_target_name := $(prorab_this_name)

$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libutki$(dot_so))

#================================
$(eval $(prorab-clear-this-vars))
#================================

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += singleton_test.cpp

this_ldlibs += $(lib_target_name)

this_no_install := true

this_ldflags += -L $(d)../../src/out/$(c)
this_ldlibs += -lutki

$(eval $(prorab-build-app))

$(prorab_this_name): $(lib_target_name)

$(eval $(call prorab-depend, $(prorab_this_name), ../../src/out/$(c)/libutki$(dot_so)))

# no test under windows, it fails there
ifneq ($(os),windows)
    this_test_cmd := $(prorab_this_name)
    this_test_deps := $(prorab_this_name)
    this_test_ld_path := out/$(c) ../../src/out/$(c)
    $(eval $(prorab-test))
endif

$(eval $(call prorab-include, ../../src/makefile))
