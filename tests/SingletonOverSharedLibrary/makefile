include prorab.mk

this_name := testso
this_soname := 0

$(eval $(call prorab-config, ../../config))

this_srcs += testso.cpp

this_no_install := true

$(eval $(prorab-build-lib))

lib_target_name := $(prorab_this_name)



$(eval $(prorab-clear-this-vars))

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += Singleton_test.cpp

this_ldlibs += $(lib_target_name)

this_no_install := true

$(eval $(prorab-build-app))

$(prorab_this_name): $(lib_target_name)

# no test under windows, it fails there
ifneq ($(os),windows)
    include $(d)../test_target.mk
endif
