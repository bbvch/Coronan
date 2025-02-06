from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CoronanRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("poco/1.13.3")
        self.requires("rapidjson/cci.20230929")
        self.requires("lyra/1.6.1")
        self.requires("catch2/3.8.0")
        self.requires("fmt/11.1.1")

    def configure(self):
        self.options["poco/*"].enable_data = False
        self.options["poco/*"].enable_data_postgresql = False
        self.options["poco/*"].enable_data_mysql = False
        self.options["poco/*"].enable_data_sqlite = False
        self.options["poco/*"].enable_data_odbc = False
        self.options["poco/*"].enable_mongodb = False
        self.options["poco/*"].enable_redis = False
        self.options["poco/*"].enable_jwt = False
        self.options["poco/*"].enable_zip = False
        self.options["poco/*"].enable_activerecord = False

    def layout(self):
        cmake_layout(self)
