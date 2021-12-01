include $(config_dir)base/base.mk

# no optimization to avoid mismamtch of actual code to source lines,
# otherwise coverage report will not be accurate
this_cxxflags += -O0

this_cxxflags += -ftest-coverage
this_cxxflags += -fprofile-arcs

# TODO: remove if not needed
# this_cxxflags += --coverage
# this_cxxflags += -fno-inline -fno-inline-small-functions -fno-default-inline

this_ldflags += --coverage

# TODO: remove if not needed
this_ldflags += -fno-inline -fno-inline-small-functions -fno-default-inline
