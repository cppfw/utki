include prorab.mk
include prorab-license.mk
include prorab-install-dbgsrc.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../config))

this_name := utki

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, $(this_name))

this_cxxflags += -isystem ../src_deps

$(eval $(prorab-build-lib))

this_src_dir := $(this_name)
this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
