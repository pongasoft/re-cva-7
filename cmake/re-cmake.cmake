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

cmake_minimum_required(VERSION 3.13)

include(FetchContent)

if(RE_CMAKE_ROOT_DIR)
  # instructs FetchContent to not download or update but use the location instead
  set(FETCHCONTENT_SOURCE_DIR_RE-CMAKE ${RE_CMAKE_ROOT_DIR})
else()
  set(FETCHCONTENT_SOURCE_DIR_RE-CMAKE "")
endif()

set(RE_CMAKE_GIT_REPO "https://github.com/pongasoft/re-cmake" CACHE STRING "re-cmake git repository url")
set(RE_CMAKE_GIT_TAG "v1.1.0" CACHE STRING "re-cmake git tag")

FetchContent_Declare(re-cmake
      GIT_REPOSITORY    ${RE_CMAKE_GIT_REPO}
      GIT_TAG           ${RE_CMAKE_GIT_TAG}
      GIT_CONFIG        advice.detachedHead=false
      GIT_SHALLOW       true
      SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/re-cmake"
      BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/re-cmake-build"
      CONFIGURE_COMMAND ""
      BUILD_COMMAND     ""
      INSTALL_COMMAND   ""
      TEST_COMMAND      ""
      )

FetchContent_GetProperties(re-cmake)

if(NOT re-cmake_POPULATED)

  if(FETCHCONTENT_SOURCE_DIR_RE-CMAKE)
    message(STATUS "Using re-cmake from local ${FETCHCONTENT_SOURCE_DIR_RE-CMAKE}")
  else()
    message(STATUS "Fetching re-cmake ${RE_CMAKE_GIT_REPO}/tree/${RE_CMAKE_GIT_TAG}")
  endif()

  FetchContent_Populate(re-cmake)

endif()

set(RE_CMAKE_ROOT_DIR ${re-cmake_SOURCE_DIR})

# finally we include the framework itself
include(${RE_CMAKE_ROOT_DIR}/sdk.cmake)
