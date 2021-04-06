include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp tests.cpp

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libutki$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
