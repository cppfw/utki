include prorab.mk
include prorab-test.mk

$(eval $(call prorab-config, ../../config))

this_no_install := true

this_name := tests

this_srcs := $(call prorab-src-dir, src)

this_libutki := ../../src/out/$(c)/libutki$(dot_so)

this_cxxflags += -I ../harness/tst/src -I ../../src
this_ldflags += -L ../harness/tst/src/out/no_par
this_ldlibs += $(this_libutki) -ltst

$(eval $(prorab-build-app))

$(eval $(call prorab-depend, $(prorab_this_name), $(this_libutki) ../harness/tst/src/out/no_par/libtst$(dot_so)))

this_test_cmd := $(prorab_this_name) --junit-out=junit.xml
this_test_deps := $(prorab_this_name)
this_test_ld_path := $(dir $(this_libutki)) ../harness/tst/src/out/no_par ../harness/clargs/src/out/rel
$(eval $(prorab-test))

$(eval $(call prorab-include, ../../src/makefile))
