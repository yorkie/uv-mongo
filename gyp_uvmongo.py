#!/usr/bin/env python

import glob
import platform
import os
import subprocess
import sys

CC = os.environ.get('CC', 'cc')
script_dir = os.path.dirname(__file__)
uvmongo_root = os.path.normpath(script_dir)
output_dir = os.path.join(os.path.abspath(uvmongo_root), 'build')

sys.path.insert(0, os.path.join(uvmongo_root, 'tools/gyp/pylib'))
try:
  import gyp
except ImportError:
  print('You need to install gyp in build/gyp first. See the README.')
  sys.exit(42)


def host_arch():
  machine = platform.machine()
  if machine == 'i386': return 'ia32'
  if machine == 'x86_64': return 'x64'
  if machine.startswith('arm'): return 'arm'
  if machine.startswith('mips'): return 'mips'
  return machine  # Return as-is and hope for the best.


def compiler_version():
  proc = subprocess.Popen(CC.split() + ['--version'], stdout=subprocess.PIPE)
  is_clang = 'clang' in proc.communicate()[0].split('\n')[0]
  proc = subprocess.Popen(CC.split() + ['-dumpversion'], stdout=subprocess.PIPE)
  version = proc.communicate()[0].split('.')
  version = map(int, version[:2])
  version = tuple(version)
  return (version, is_clang)


def run_gyp(args):
  rc = gyp.main(args)
  if rc != 0:
    print 'Error running GYP'
    sys.exit(rc)


if __name__ == '__main__':
  args = sys.argv[1:]

  args.append(os.path.join(os.path.abspath(uvmongo_root), 'uvmongo.gyp'))
  args.append('--depth=' + uvmongo_root)

  # There's a bug with windows which doesn't allow this feature.
  if sys.platform != 'win32':
    if '-f' not in args:
      args.extend('-f make'.split())
    if 'eclipse' not in args and 'ninja' not in args:
      args.extend(['-Goutput_dir=' + output_dir])
      args.extend(['--generator-output', output_dir])
    (major, minor), is_clang = compiler_version()
    args.append('-Dgcc_version=%d' % (10 * major + minor))
    args.append('-Dclang=%d' % int(is_clang))

  if not any(a.startswith('-Dhost_arch=') for a in args):
    args.append('-Dhost_arch=%s' % host_arch())

  if not any(a.startswith('-Dtarget_arch=') for a in args):
    args.append('-Dtarget_arch=%s' % host_arch())

  if not any(a.startswith('-Dlibrary=') for a in args):
    args.append('-Dlibrary=static_library')

  if not any(a.startswith('-Dcomponent=') for a in args):
    args.append('-Dcomponent=static_library')

  gyp_args = list(args)
  print gyp_args
  run_gyp(gyp_args)
