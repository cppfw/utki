include prorab.mk

this_name := utki

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

$(eval $(call prorab-config, ../config))

$(eval $(prorab-build-lib))
