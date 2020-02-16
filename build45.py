# Build configuration

# Path to the Jukebox SDK directory. A relative or an absolute path is allowed
import jukebox

# List all C or C++ files here. Wildcards, relative and absolute paths are allowed
SOURCE_FILES = [
	"%s/src/cpp/*.cpp" % (jukebox.RE_COMMON_ROOT_DIR),
	"src/cpp/*.cpp"
	]

# List any extra include directories for clang. Relative and absolute paths are allowed
INCLUDE_DIRS = [
	"%s/src/cpp" % (jukebox.RE_COMMON_ROOT_DIR),
	]

# Other compiler flags, for example warning flags like -Wconversion, or preprocessor defines like -DDEBUG=1
OTHER_COMPILER_FLAGS = "-Wall -Wno-c++11-extensions"

VERBOSE_BUILD = True

# End of build configuration








# Add the build directory to the python path to be able to find the imports below
import os
import sys
importPath = os.path.abspath(os.path.join(jukebox.JUKEBOX_SDK_DIR, "Tools", "Build"))
sys.path.append(importPath)

# Propagate variables to the main build script
import buildconfig
buildconfig.SOURCE_FILES = SOURCE_FILES
buildconfig.JUKEBOX_SDK_DIR = os.path.normpath(jukebox.JUKEBOX_SDK_DIR)
buildconfig.INCLUDE_DIRS = INCLUDE_DIRS
buildconfig.OTHER_COMPILER_FLAGS = OTHER_COMPILER_FLAGS
buildconfig.VERBOSE_BUILD = VERBOSE_BUILD

# Clang's static code analysis is turned on by default. Uncomment to temporarily turn off.
# buildconfig.STATIC_ANALYSIS = False

# Default is low verbosity during builds. All errors and warnings are still visible though.
# buildconfig.VERBOSE_BUILD = True

# Start the main build script
import build
build.doBuild(sys.argv)
