include prorab.mk

this_name := utki

this_soname := 0

this_srcs := $(call prorab-src-dir, utki)

$(eval $(call prorab-config, ../config))

this_ldlibs += -lstdc++

this_out_dir := build

$(eval $(prorab-build-lib))
