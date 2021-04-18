import os

from conans import ConanFile, CMake, tools, RunEnvironment


class UtkiTestConan(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	generators = "cmake"

	def build(self):
		cmake = CMake(self)
		# Current dir is "test_package/build/<build_id>" and CMakeLists.txt is
		# in "test_package"
		cmake.configure()
		cmake.build()

	def imports(self):
		self.copy("*.dll", dst="bin", src="bin")
		self.copy("*.dylib*", dst="bin", src="lib")
		self.copy('*.so*', dst='bin', src='lib')

	def test(self):
		if not tools.cross_building(self):
			env_build = RunEnvironment(self)
			os.chdir("bin")
			with tools.environment_append(env_build.vars):
				self.run("DYLD_LIBRARY_PATH=%s echo DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH" % env_build.vars["DYLD_LIBRARY_PATH"])
				self.run("DYLD_LIBRARY_PATH=%s ls $DYLD_LIBRARY_PATH" % env_build.vars["DYLD_LIBRARY_PATH"])
				self.run("DYLD_LIBRARY_PATH=%s .%sexample" % (env_build.vars["DYLD_LIBRARY_PATH"], os.sep))
