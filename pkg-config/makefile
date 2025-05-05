include prorab.mk
include prorab-pkg-config.mk

$(eval $(call prorab-config, ../config))

ifneq ($(config),dbg)

    this_version := $(shell myci-deb-version.sh)

    $(eval $(prorab-pkg-config))

endif
