include prorab.mk

$(eval $(call prorab-include, ../../src/makefile))

clargs_cfg := rel_no_install
tst_cfg := no_par_no_install

# load config in order to resolve 'c' and 'config' in case of default config
$(eval $(call prorab-config, ../../config))

$(eval $(call prorab-depend, \
		clargs/src/out/$(clargs_cfg)/libclargs$(dot_so), \
		../../src/out/$(c)/libutki$(this_dbg)$(dot_so) \
	))
$(eval $(call prorab-depend, \
		tst/src/out/$(tst_cfg)/libtst$(dot_so), \
		clargs/src/out/$(clargs_cfg)/libclargs$(dot_so) \
		../../src/out/$(c)/libutki$(this_dbg)$(dot_so) \
	))

harness_old_cxxflags := $(CXXFLAGS)
harness_old_ldflags := $(LDFLAGS)
CXXFLAGS += -I$(d)../../src -I$(d)clargs/src
LDFLAGS += -L$(d)../../src/out/$(c) -L$(d)clargs/src/out/$(clargs_cfg)
$(eval $(call prorab-include, clargs/src/makefile, $(clargs_cfg)))
$(eval $(call prorab-include, tst/src/makefile, $(tst_cfg)))
CXXFLAGS := $(harness_old_cxxflags)
LDFLAGS := $(harness_old_ldflags)
