from distutils.core import setup, Extension

UnixSockets = Extension('UnixSockets',
                        sources = ['src/python/unixsockets.c'],
                        library_dirs = ['unix_sockets/'],
                        libraries = ['client'],
                        include_dirs = ['unix_sockets/src/include/'])

setup(name = 'UnixSockets',
      version = '1.0',
      description = 'Unix sockets library for python',
      ext_modules = [UnixSockets])

