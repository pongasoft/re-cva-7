# Path to the Jukebox SDK directory. A relative or an absolute path is allowed
import jukebox

import subprocess
import os
import sys

pwd = os.path.dirname(os.path.realpath(__file__))
RE2DRender = os.path.join(jukebox.JUKEBOX_ROOT_DIR, "RE2DRender", "RE2DRender.app", "Contents", "MacOS", "RE2DRender")
GUI2D = os.path.join(pwd, "GUI2D")
GUI = os.path.join(pwd, "GUI")

print RE2DRender + " " + GUI2D + " " + GUI

sys.exit(subprocess.call([RE2DRender, GUI2D, GUI]))
