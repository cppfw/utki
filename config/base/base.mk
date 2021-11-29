# this_cxx := clang++-11

this_cxxflags += -Wall # enable all warnings
this_cxxflags += -Wno-comment # enable all warnings
this_cxxflags += -Wnon-virtual-dtor # warn if base class has non-virtual destructor
this_cxxflags += -Werror # treat warnings as errors
# this_cxxflags += -Wfatal-errors # stop on first error encountered
this_cxxflags += -fstrict-aliasing # in order to comply with the c++ standard more strictly
this_cxxflags += -std=c++17
this_cxxflags += -fPIC
this_cxxflags += -g

this_ldlibs += -lstdc++
