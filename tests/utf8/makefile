include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp tests.cpp

this_ldlibs += -lutki

this_ldflags += -L$(d)../../src/out/$(c)

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

# add dependency on libutki
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libutki$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
