from conans import ConanFile, CMake, tools


class DeepinbootmakerConan(ConanFile):
    name = "deepin-boot-maker"
    version = "2.0.4.8"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Deepinbootmaker here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "qmake"
    requires = 'dtkcore/2.0.9@iceyer/stable', 'dtkwidget/2.0.9@iceyer/stable', 'OpenSSL/1.0.2n@conan/stable', 'jom_installer/1.1.2@bincrafters/stable'

    def build(self):
        outdir = self.build_folder

        env_vars = tools.vcvars_dict(self.settings)
        env_vars['_CL_'] = '/utf-8'
        with tools.environment_append(env_vars):
            command = 'qmake -r'
            command += ' VERSION=%s' % self.version
            # command += ' CONFIG-=debug_and_release'
            # command += ' CONFIG-=debug_and_release_target'
            command += ' CONFIG+=release'
            command += ' PREFIX=%s' % outdir
            command += ' DEFINES+=DTK_STATIC_LIB'
            command += ' DTK_STATIC_TRANSLATION=YES'
            command += ' DTK_NO_MULTIMEDIA=YES'
            command += ' %s' % self.source_folder
            self.run(command)
            # self.run('jom clean')
            self.run('jom')
            self.run('jom install')

    def package(self):
        self.copy("*.h", dst="include", src="hello")

    def package_info(self):
        self.cpp_info.libs = ["hello"]

