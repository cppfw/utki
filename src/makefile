include prorab.mk

this_name := utki

this_soname := 0

this_srcs := $(call prorab-src-dir, utki)

this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += -fstrict-aliasing
this_cxxflags += -g
this_cxxflags += -fPIC

this_ldlibs += -lstdc++

ifeq ($(debug),true)
    this_cxxflags += -DDEBUG
else
	this_cxxflags += -O3
endif

this_out_dir := build

$(eval $(prorab-build-lib))
