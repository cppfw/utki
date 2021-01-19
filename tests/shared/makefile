include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += $(call prorab-src-dir, .)

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

