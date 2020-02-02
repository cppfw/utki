include prorab.mk

this_name := testso
this_soname := 0

this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += #-DDEBUG
this_cxxflags += -fstrict-aliasing
this_cxxflags += -O3

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

this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing

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
define this_rules
test:: $(prorab_this_name)
	@myci-running-test.sh $$(notdir $$(patsubst %/, %, $$(dir $$^)))
	@(cd $(d); $(if $(filter windows,$(os)), $$^, LD_LIBRARY_PATH=. $$^))
	@myci-passed.sh
endef
$(eval $(this_rules))
endif

