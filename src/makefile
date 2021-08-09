include prorab.mk
include prorab-license.mk

this_name := utki

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

$(eval $(call prorab-config, ../config))

$(eval $(prorab-build-lib))

this_src_dir := 
this_license_file := ../LICENSE
$(eval $(prorab-license))
