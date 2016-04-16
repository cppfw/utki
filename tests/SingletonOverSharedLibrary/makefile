include prorab.mk



this_name := testso
this_soname := 0

this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += #-DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -O3

ifeq ($(prorab_os),macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
else ifeq ($(prorab_os),windows)
else
    this_cxxflags += -fPIC
endif

this_srcs += testso.cpp

$(eval $(prorab-build-lib))

lib_target_name := $(prorab_this_name)



$(prorab-clear-this-vars)



this_name := tests


#compiler flags
this_cxxflags += -std=c++11
this_cxxflags += -Wall
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!

this_srcs += Singleton_test.cpp

this_ldlibs += $(lib_target_name)

ifeq ($(prorab_os),macosx)
    this_cxxflags += -stdlib=libc++ #this is needed to be able to use c++11 std lib
    this_ldlibs += -lc++
endif

$(eval $(prorab-build-app))

$(prorab_this_name): $(lib_target_name)


#no test under windows, it will fails there
ifneq ($(prorab_os),windows)
define this_rules
test:: $(prorab_this_name)
	@echo running $$^...
	@(cd $(prorab_this_dir); $(if $(filter windows,$(prorab_os)), $$^, LD_LIBRARY_PATH=. $$^))
endef
$(eval $(this_rules))
endif

