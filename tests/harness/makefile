include prorab.mk

$(eval $(call prorab-config, ../../config))
$(eval $(call prorab-depend, \
		clargs/src/out/rel/libclargs$(dot_so).$(shell cat $(d)clargs/src/soname.txt), \
		../../src/out/$(c)/libutki$(dot_so) \
	))
$(eval $(call prorab-depend, \
		tst/src/out/no_par/libtst$(dot_so).$(shell cat $(d)tst/src/soname.txt), \
		clargs/src/out/rel/libclargs$(dot_so) \
		../../src/out/$(c)/libutki$(dot_so) \
	))

harness_old_cxxflags := $(CXXFLAGS)
harness_old_ldflags := $(LDLFAGS)
CXXFLAGS += -I$(d)../../src -I$(d)clargs/src
LDFLAGS += -L$(d)../../src/out/$(c) -L$(d)clargs/src/out/rel
$(eval $(call prorab-include, clargs/src/makefile, rel))
$(eval $(call prorab-include, tst/src/makefile, no_par))
CXXFLAGS := $(harness_old_cxxflags)
LDFLAGS := $(harness_old_ldflags)
