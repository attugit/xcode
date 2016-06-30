#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='xCode'

top = '.'
out = 'build'
flags = [
  '-std=c++1z',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  '-Wunused',
  '-pedantic',
  '-pedantic-errors',
  '-Werror'
]

def init(ctx):
  ctx.load('build_logs')

def options(opt):
  opt.load('compiler_cxx')
  opt.load('boost')
  opt.load('waf_unit_test')

def configure(conf):
  conf.load('compiler_cxx')
  conf.load('boost')
  conf.check_boost()
  conf.load('waf_unit_test')

import os
from waflib.Tools import waf_unit_test
def build(bld):
  if not bld.variant:
    bld.fatal('try "waf --help"')
  bld.env.INCLUDES += ['.', bld.bldnode.abspath()]
  bld.env.CXXFLAGS += flags
  if bld.variant == 'debug':
    bld.env.CXXFLAGS += ['-g', '-O0']
    bld.env.DEFINES += ['DEBUG']
  if bld.variant == 'release':
    bld.env.CXXFLAGS += ['-O3', '-march=native', '-fPIC', '-fno-rtti']
    bld.env.DEFINES += ['NDEBUG']
  bld(
    source       = bld.path.ant_glob(['src/lib/**/*.cpp']),
    target       = 'xc',
    features     = 'cxx cxxshlib',
  )
  bld(
    source       = bld.path.ant_glob(['src/bin/**/*.cpp']),
    target       = APPNAME,
    features     = 'cxx cxxprogram',
    use          = ['xc'],
  )
  bld.env.INCLUDES += ['gtest', 'gtest/include']
  bld(
    source       = bld.path.ant_glob(['gtest/src/gtest-all.cc', 'gtest/src/gtest_main.cc']),
    target       = 'GTEST',
    features     = 'cxx',
    install_path = None,
  )
  bld.env.INCLUDES += ['gmock', 'gmock/include']
  bld.env.LIB += ['pthread']
  for suite in os.listdir('./test'):
    if 'ignore' not in suite and os.path.isdir(os.path.join('./test', suite)):
      bld(
        source       = bld.path.ant_glob(['test/%s/**/*.cpp'%suite]),
        target       = 'ut_' + str(suite),
        features     = 'cxx cxxprogram test',
        use          = ['xc', 'GTEST', 'GMOCK'],
        install_path = None,
      )
  bld.add_post_fun(waf_unit_test.summary)
  bld.add_post_fun(waf_unit_test.set_exit_code)
  inc = bld.path.find_dir('src/lib')
  for f in bld.path.ant_glob(['src/lib/**/*.hpp']):
    bld.install_files('${PREFIX}/include/' + f.path_from(inc).replace(f.name, ''), f)

from waflib.Build import BuildContext
from waflib.Build import CleanContext
from waflib.Build import InstallContext
from waflib.Build import UninstallContext

for ctx in (BuildContext, CleanContext, InstallContext, UninstallContext):
  name = ctx.__name__.replace('Context','').lower()
  class debug(ctx):
    cmd = name + '_debug'
    variant = 'debug'
  class release(ctx):
    cmd = name + '_release'
    variant = 'release'

