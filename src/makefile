include prorab.mk
include prorab-license.mk
include prorab-install-dbgsrc.mk
include prorab-clang-tidy.mk

$(eval $(call prorab-config, ../config))

this_name := utki

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

$(eval $(prorab-build-lib))

$(eval $(prorab-clang-tidy))

this_src_dir := $(this_name)
this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-install-dbgsrc))
