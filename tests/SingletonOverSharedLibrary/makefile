include prorab.mk

this_name := testso
this_soname := 0

$(eval $(call prorab-config, ../../config))

ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(os),windows)
    this_ldlibs += -lstdc++
else
    this_cxxflags += -fPIC
endif

this_srcs += testso.cpp

this_no_install := true

$(eval $(prorab-build-lib))

lib_target_name := $(prorab_this_name)



$(eval $(prorab-clear-this-vars))

this_name := tests

$(eval $(call prorab-config, ../../config))

this_srcs += Singleton_test.cpp

this_ldlibs += $(lib_target_name) -lstdc++

ifeq ($(os),macosx)
    this_cxxflags += -stdlib=libc++ # this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
endif

this_no_install := true

$(eval $(prorab-build-app))

$(prorab_this_name): $(lib_target_name)

# no test under windows, it fails there
ifneq ($(os),windows)
    include $(d)../test_target.mk
endif
