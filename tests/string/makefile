include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp

ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
endif

this_ldflags += -L $(d)../../src/out/$(c)

this_ldlibs += -lutki -lstdc++

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libutki$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
