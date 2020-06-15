#!/usr/bin/env python3

# Copyright (c) 2020 pongasoft
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#
# @author Yan Pujante

import argparse
import os
import sys
import platform

parser = argparse.ArgumentParser(allow_abbrev=False,
                                 usage='configure.py [-h] [-n] [-f] [-R] [-p RE_SDK_ROOT] [-r RE_2D_RENDER_ROOT] [-G GENERATOR] [-- <cmake_options>]',
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog='''
Notes
  -p defaults to /Users/Shared/ReasonStudios/JukeboxSDK_<RE_SDK_VERSION>/SDK
  
  -r defaults to <path_to_sdk>/../RE2DRender
  
  -G defaults to "Unix Makefiles" on macOS and "Visual Studio 16 2019" / X64 for Windows10
  run 'cmake --help' to get the list of generators supported
  
  For single-config generators, Debug is used by default and can be changed with -R for Release
  For multi-config generators, -R is ignored
   
  To provide extra options to CMake you do it this way
  python3 configure.py -- -Wdev

Examples
  # Specify an explicit path to the sdk and uses default generator
  python3 configure.py -p /opt/local/JukeboxSDK_4.2.0/SDK

  # Use default sdk path and uses Xcode generator
  python3 configure.py -G Xcode

  # Use defaults
  python3 configure.py
''')
parser.add_argument("-n", "--dry-run", help="Dry run (prints what it is going to do)", action="store_true", dest="dry_run")
parser.add_argument("-f", "--force", help="Force a regeneration (delete and recreate build folder)", action="store_true")
parser.add_argument("-p", "--sdk-path", help="Path to the sdk (optional)", dest="re_sdk_root")
parser.add_argument("-r", "--render-path", help="Path to RE2DRender (optional)", dest="re_2d_render_root")
parser.add_argument("-G", "--generator", help="CMake generator (optional)")
parser.add_argument("-R", "--release", help="Use CMake Release build type (for single-config generators)", action="store_true")
parser.add_argument('cmake_options', help="Any options for cmake", nargs=argparse.REMAINDER)

args = parser.parse_args()

# The CMakeLists.txt file is a sibling of this script
this_script_root_dir = os.path.dirname(os.path.realpath(sys.argv[0]))

# RE_SDK_ROOT
re_sdk_root = [] if not args.re_sdk_root else [f'-DRE_SDK_ROOT:PATH={args.re_sdk_root}']

# RE_2D_RENDER_ROOT
re_2d_render_root = [] if not args.re_2d_render_root else [f'-DRE_2D_RENDER_ROOT:PATH={args.re_2d_render_root}']

# CMake generator
cmake_generator = ['-G']
if args.generator:
    cmake_generator.append(args.generator)
else:
    if platform.system() == 'Darwin':
        cmake_generator.append('Unix Makefiles')
    else:
        cmake_generator.append('Visual Studio 16 2019')
        cmake_generator.extend(['-A', 'X64'])

# CMake options
cmake_options = [] if not args.cmake_options else args.cmake_options[1:]

# CMake build type (for single config generators)
cmake_build_type = [f'-DCMAKE_BUILD_TYPE={"Release" if args.release else "Debug"}']

# CMake build directory (a subdirectory to wherever the command is run)
cmake_build_dir = ['-B', 'build']

# CMake command
cmake_command = ['cmake',
                 *cmake_build_dir,
                 *re_sdk_root, *re_2d_render_root,
                 *cmake_build_type,
                 *cmake_generator,
                 *cmake_options,
                 this_script_root_dir]

if args.dry_run:
    escaped_command = ' '.join([f'"{x}"' for x in cmake_command[1:]])
    print(f'cmake {escaped_command}')
else:
    if args.force:
        import shutil
        if os.path.exists('build'):
            shutil.rmtree('build')

    import subprocess
    subprocess.run(cmake_command)





