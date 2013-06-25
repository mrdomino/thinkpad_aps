from distutils.core import setup, Extension

module1 = Extension('aps', sources = ['aps.c', 'pyaps.c'])

setup(name = 'thinkpad_aps',
      version = '0.0',
      description = 'Thinkpad APS accelerometer data',
      ext_modules = [module1])
