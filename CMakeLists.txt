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

cmake_minimum_required(VERSION 3.24)

# Capturing root of project
set(RE_PROJECT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

# No HD custom display
set(RE_CMAKE_RE_2D_RENDER_HI_RES_OPTION "hi-res" CACHE STRING "Option for invoking RE2DRender for hi res build (hi-res or hi-res-only)")

# Using RE SDK version 4.6.0
set(RE_SDK_VERSION 4.6.0)

include(cmake/options.cmake)  # (optional) override versions used
include(cmake/re-cmake.cmake) # include re-cmake (BEFORE project() definition)

# Setting up project (note that the version is defined in lua.info)
project(re-cva-7 LANGUAGES CXX)

# Using C++17
set(CMAKE_CXX_STANDARD 17)

# Initializes re-cmake (includes re-logging/loguru for logging and re-mock for unit testing)
re_cmake_init(INCLUDES re-logging re-mock)

# re-common
fetch_content(NAME re-common)
include("${re-common_ROOT_DIR}/re-common.cmake")

# Defining where the sources of the plugin live
set(RE_CPP_SRC_DIR "${RE_PROJECT_ROOT_DIR}/src/cpp")

# Defining all the source files for the project
# Although you could use a file(GLOB ...) pattern, it is NOT recommended
# as the CMake generation would happen on every single build!
set(re_sources_cpp
    ${RE_CPP_SRC_DIR}/Device.cpp
    ${RE_CPP_SRC_DIR}/DeviceFactory.cpp
    ${RE_CPP_SRC_DIR}/DeviceState.cpp
    ${RE_CPP_SRC_DIR}/Motherboard.cpp
    ${RE_CPP_SRC_DIR}/ZoomWindow.cpp
)

# Defining where the local GUI2D files live
set(RE_2D_SRC_DIR "${RE_PROJECT_ROOT_DIR}/GUI2D")

# Defining all the 2D source files
# Although you could use a file(GLOB ...) pattern, it is NOT recommended as the CMake generation would happen on
# every single build!
set(re_sources_2d

    # lua files describing the GUI
    ${RE_2D_SRC_DIR}/device_2D.lua
    ${RE_2D_SRC_DIR}/hdgui_2D.lua

    # Images for the device
    ${RE_2D_SRC_DIR}/Button_mini_translucent_2frames.png
    ${RE_2D_SRC_DIR}/Button_mini_translucent_4frames.png
    ${RE_2D_SRC_DIR}/Cable_Attachment_CV_01_1frames.png
    ${RE_2D_SRC_DIR}/Display_CV_Note_Landscape.png
    ${RE_2D_SRC_DIR}/Display_CV_Note_Landscape_Background.png
    ${RE_2D_SRC_DIR}/Display_CV_Note_Portrait.png
    ${RE_2D_SRC_DIR}/Display_CV_Note_Portrait_Background.png
    ${RE_2D_SRC_DIR}/Display_CV_Value.png
    ${RE_2D_SRC_DIR}/Display_CV_Value_As.png
    ${RE_2D_SRC_DIR}/Display_CV_Value_As_Background.png
    ${RE_2D_SRC_DIR}/Display_CV_Value_Background.png
    ${RE_2D_SRC_DIR}/InputPaused_4frames.png
    ${RE_2D_SRC_DIR}/Knob_01_mini_63frames.png
    ${RE_2D_SRC_DIR}/Knob_17_matte_63frames.png
    ${RE_2D_SRC_DIR}/Main_LCD.png
    ${RE_2D_SRC_DIR}/Main_LCD_Background.png
    ${RE_2D_SRC_DIR}/Main_LCD_Scrollbar.png
    ${RE_2D_SRC_DIR}/Main_LCD_Scrollbar_Background.png
    ${RE_2D_SRC_DIR}/Panel_Back.png
    ${RE_2D_SRC_DIR}/Panel_Folded_Back.png
    ${RE_2D_SRC_DIR}/Panel_Folded_Front.png
    ${RE_2D_SRC_DIR}/Panel_Front.png
    ${RE_2D_SRC_DIR}/Placeholder.png
    ${RE_2D_SRC_DIR}/Tape_Horizontal_1frames.png
    ${RE_2D_SRC_DIR}/Tape_Vertical_1frames.png
    ${RE_2D_SRC_DIR}/Toggle_Fader_3frames.png
    ${RE_2D_SRC_DIR}/Toggle_Fader_4frames.png
    ${RE_2D_SRC_DIR}/TrimKnob.png
)

# Location of the test cases
set(TEST_DIR "${RE_PROJECT_ROOT_DIR}/test/cpp")

# List of test cases
set(re_test_cpp
    "${TEST_DIR}/test-Device.cpp"
)

# Adding the RE plugin
add_re_plugin(
    RE_SDK_VERSION         "${RE_SDK_VERSION}"
    RE_SDK_ROOT            "${RE_SDK_ROOT}"
    RE_2D_RENDER_ROOT      "${RE_2D_RENDER_ROOT}"
    BUILD_SOURCES          "${re_sources_cpp}" "${re-common_BUILD_SOURCES}"
    NATIVE_BUILD_SOURCES   "${re-logging_SOURCES}" "${re-common_NATIVE_BUILD_SOURCES}"
    RENDER_2D_SOURCES      "${re_sources_2d}"
    INCLUDE_DIRECTORIES    "${re-logging_INCLUDE_DIRS}" "${re-common_INCLUDE_DIRECTORIES}" # plugin uses loguru
    #   COMPILE_OPTIONS        -Wall
    #   NATIVE_COMPILE_OPTIONS -Wall                     # Ok for macOS, try -W4 instead for Windows/Clang
    JBOX_COMPILE_OPTIONS   -Wall
    ENABLE_DEBUG_LOGGING                             # turn on JBOX_TRACE and loguru

    # Testing
    TEST_CASE_SOURCES        "${re_test_cpp}"        # the source files containing the test cases
    TEST_INCLUDE_DIRECTORIES "${RE_CPP_SRC_DIR}"     # tests include plugin classes
    TEST_LINK_LIBS           "native-test-lib" "${re-mock_LIBRARY_NAME}"  # tests link with plugin classes and re-mock
)
