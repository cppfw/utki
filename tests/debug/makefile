include prorab.mk

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += main.cpp tests.cpp

ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(os),windows)
else
    this_ldlibs += -lpthread
endif

this_ldflags += -L$(d)../../src/

this_ldlibs += -lstdc++

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk
