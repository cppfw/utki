include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp tests.cpp

this_ldlibs += -lutki -lstdc++

this_ldflags += -L$(d)../../src/out/$(c)

ifeq ($(os),linux)
    this_cxxflags += -fPIC
else ifeq ($(os),macosx)
    this_cxxflags += -fPIC
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(os),windows)
endif

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

# add dependency on libutki
$(prorab_this_name): $(abspath $(d)../../src/out/$(c)/libutki$(dot_so))

$(eval $(call prorab-include, ../../src/makefile))
