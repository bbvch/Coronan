from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CoronanRecipe(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeDeps'

    def configure(self):
        self.options['poco'].enable_data = False
        self.options['poco'].enable_data_postgresql = False
        self.options['poco'].enable_data_mysql = False
        self.options['poco'].enable_data_sqlite = False
        self.options['poco'].enable_data_odbc = False
        self.options['poco'].enable_mongodb = False
        self.options['poco'].enable_redis = False
        self.options['poco'].enable_jwt = False
        self.options['poco'].enable_zip = False
        self.options['poco'].enable_activerecord = False


    def requirements(self):
        self.requires('poco/1.14.2')
        self.requires('rapidjson/cci.20230929')
        self.requires('lyra/1.7.0')
        self.requires('catch2/3.11.0')
        self.requires('fmt/12.1.0')
        self.requires('date/3.0.4')


    def layout(self):
        cmake_layout(self)
