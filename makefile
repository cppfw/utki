include prorab.mk


$(eval $(prorab-build-subdirs))

$(prorab-clear-this-vars)


$(eval $(prorab-build-deb))

