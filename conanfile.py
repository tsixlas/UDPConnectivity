from os import path
from conan import ConanFile
from conan.tools.cmake import CMake
from conan.tools.files import copy


class UDPConnectivityRecipe(ConanFile):
    name = "udp.connectivity"
    version = "0.1"
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "src/*.cpp", "include/*.h"
    package_type = "application"

    def requirements(self):
        self.test_requires("gtest/1.16.0")

    def layout(self):
        build_path = path.join("cmake-build-" + str(self.settings.build_type).lower())
        self.folders.generators = build_path
        self.folders.build = build_path
        self.folders.install = build_path

    def build(self):
        self.cmake = CMake(self)
        self.cmake.configure()
        self.cmake.build()
        #self.cmake.test()
        self.output.success(self.settings.build_type)

    def package(self):
        self.cmake.install()