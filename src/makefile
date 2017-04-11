include prorab.mk

this_name := utki

this_soname := 0

$(eval ph := $(patsubst $(d)%,%,$(shell find $(d) -type f -name "*.hpp" -o -name "*.h")))
$(info ph = $(ph))

$(eval $(prorab-build-lib))
