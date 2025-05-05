include prorab.mk
include prorab-license.mk
include prorab-install-dbgsrc.mk
include prorab-clang-format.mk

$(eval $(call prorab-config, ../config))

this__src_dir := utki

this_name := $(this__src_dir)$(this_dbg)

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, $(this__src_dir))

this_cxxflags += -isystem ../src_deps

$(eval $(prorab-build-lib))

this_src_dir := $(this__src_dir)
this_license_file := ../LICENSE
$(eval $(prorab-license))

$(eval $(prorab-clang-format))

$(eval $(prorab-install-dbgsrc))
