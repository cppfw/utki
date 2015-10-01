include prorab.mk


$(eval $(prorab-build-subdirs))

$(prorab-clear-this-vars)


$(eval $(prorab-build-deb))

$(prorab-clear-this-vars)


this_version_files += doc/doxygen.cfg.in
this_version_files += nuget.autopkg.in

$(eval $(prorab-apply-version))
