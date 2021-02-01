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

if(RE_COMMON_ROOT_DIR)
  # instructs FetchContent to not download or update but use the location instead
  set(FETCHCONTENT_SOURCE_DIR_RE-COMMON ${RE_COMMON_ROOT_DIR})
else()
  set(FETCHCONTENT_SOURCE_DIR_RE-COMMON "")
endif()

set(RE_COMMON_GIT_REPO "https://github.com/pongasoft/re-common" CACHE STRING "re-common git repository url")
set(RE_COMMON_GIT_TAG "v3.0.0" CACHE STRING "re-common git tag")

FetchContent_Declare(re-common
      GIT_REPOSITORY    ${RE_COMMON_GIT_REPO}
      GIT_TAG           ${RE_COMMON_GIT_TAG}
      GIT_CONFIG        advice.detachedHead=false
      GIT_SHALLOW       true
      SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/re-common"
      BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/re-common-build"
      CONFIGURE_COMMAND ""
      BUILD_COMMAND     ""
      INSTALL_COMMAND   ""
      TEST_COMMAND      ""
      )

FetchContent_GetProperties(re-common)

if(NOT re-common_POPULATED)

  if(FETCHCONTENT_SOURCE_DIR_RE-COMMON)
    message(STATUS "Using re-common from local ${FETCHCONTENT_SOURCE_DIR_RE-COMMON}")
  else()
    message(STATUS "Fetching re-common ${RE_COMMON_GIT_REPO}/tree/${RE_COMMON_GIT_TAG}")
  endif()

  FetchContent_Populate(re-common)

endif()

set(RE_COMMON_ROOT_DIR ${re-common_SOURCE_DIR})

# finally we include the framework itself
include(${RE_COMMON_ROOT_DIR}/re-common.cmake)
