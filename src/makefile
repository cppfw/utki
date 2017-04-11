include prorab.mk

this_name := utki

this_soname := 0

rwildcard = $(foreach dd,$(wildcard $1*),$(call rwildcard,$(dd)/,$2) $(filter $(subst *,%,$2),$(dd)))

$(info rw = $(call rwildcard, $(d), *.h *.hpp))

$(eval ph := $(patsubst $(d)%,%,$(call rwildcard, $(d), *.h *.hpp)))
$(info ph = $(ph))

$(eval $(prorab-build-lib))
