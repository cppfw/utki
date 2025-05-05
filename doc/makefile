include prorab.mk
include prorab-doxygen.mk

$(eval $(call prorab-config, ../config))

ifneq ($(config),dbg)

    this_name := utki
    this_out_dir := out
    this_version := $(shell myci-deb-version.sh)

    $(eval $(prorab-build-doxygen))

endif
