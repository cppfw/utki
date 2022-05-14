include prorab.mk
include prorab-license.mk
include prorab-install-dbgsrc.mk

$(eval $(call prorab-config, ../config))

this_name := utki

this_soname := $(shell cat $(d)soname.txt)

this_srcs := $(call prorab-src-dir, .)

$(eval $(prorab-build-lib))

this_src_dir := $(this_name)
this_license_file := ../LICENSE
$(eval $(prorab-license))

# this_src_suffixes := c h cpp hpp cxx hxx

define this_rules

    $(eval prorab_private_src_suffixes := $(if $(this_src_suffixes),$(this_src_suffixes),c h cpp hpp cxx hxx))
    $(eval prorab_private_src_dir := $(if $(this_src_dir),$(this_src_dir),.))

    $(eval prorab_private_format_find_patterns := -name *.$(firstword $(prorab_private_src_suffixes)) \
            $(foreach s,$(wordlist 2,$(words $(prorab_private_src_suffixes)),$(prorab_private_src_suffixes)),-or -name *.$(s)))
    $(eval prorab_private_format_srcs := $(shell cd $(d) && find $(prorab_private_src_dir) -type f $(prorab_private_format_find_patterns)))

    test::
$(.RECIPEPREFIX)@echo "check format"
$(.RECIPEPREFIX)$(a)(cd $(d) && clang-format --dry-run --Werror $(prorab_private_format_srcs))

    apply-format:
$(.RECIPEPREFIX)@echo "apply format"
$(.RECIPEPREFIX)$(a)(cd $(d) && clang-format -i --Werror $(prorab_private_format_srcs))

endef
$(eval $(this_rules))
# $(info prorab_private_format_find_patterns = $(prorab_private_format_find_patterns))
# $(info prorab_private_format_srcs = $(prorab_private_format_srcs))

$(eval $(prorab-install-dbgsrc))
