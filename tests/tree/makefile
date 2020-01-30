include prorab.mk

this_name := tests

# compiler flags
this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += -Werror
this_cxxflags += -fstrict-aliasing
this_cxxflags += -g

ifeq ($(debug),true)
	this_cxxflags += -DDEBUG
else
	this_cxxflags += -O3
endif

this_srcs += main.cpp tests.cpp

ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
endif

this_ldlibs += -lstdc++

this_no_install := true

$(eval $(prorab-build-app))

include $(d)../test_target.mk

