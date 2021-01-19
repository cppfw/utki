this_cxxflags += -Wall # enable all warnings
this_cxxflags += -Wnon-virtual-dtor # warn if base class has non-virtual destructor
this_cxxflags += -Werror # treat warnings as errors
# this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing # in order to comply with the c++ standard more strictly
this_cxxflags += -std=c++17
this_cxxflags += -fPIC
this_cxxflags += -g

ifeq ($(os),macosx)
    # this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    # this_ldlibs += -lc++
	this_ldlibs += -lstdc++
else
	this_ldlibs += -lstdc++
endif

ifeq ($(gprof), true)
    this_cxxflags += -pg
    this_ldflags += -pg
endif
