# Copyright (c) 2025 pongasoft
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

#################
# re-cmake
#################
set(re-cmake_GIT_TAG "v1.8.4" CACHE STRING "re-cmake git tag")
set(re-cmake_GIT_REPO "https://github.com/pongasoft/re-cmake" CACHE STRING "re-cmake git repository url")
set(re-cmake_DOWNLOAD_URL "${re-cmake_GIT_REPO}/archive/refs/tags/${re-cmake_GIT_TAG}.zip" CACHE STRING "re-cmake download url" FORCE)
set(re-cmake_DOWNLOAD_URL_HASH "SHA256=4f3c815cb6018c28c983509bfb53016f31c5f99d4f451e5c4edf4146bb0f2ca2" CACHE STRING "re-cmake download url hash" FORCE)

# Uncomment/set to proper value if local instead of remote
set(re-cmake_ROOT_DIR "") # use remote re-cmake
#set(re-cmake_ROOT_DIR "${RE_PROJECT_ROOT_DIR}/../../pongasoft/re-cmake")

#################
# re-logging
#################
set(re-logging_GIT_REPO "https://github.com/pongasoft/re-logging" CACHE STRING "re-logging git repository url")
set(re-logging_GIT_TAG "v2.0.2" CACHE STRING "re-logging git tag")
set(re-logging_DOWNLOAD_URL "${re-logging_GIT_REPO}/archive/refs/tags/${re-logging_GIT_TAG}.zip" CACHE STRING "re-logging download url" FORCE)
set(re-logging_DOWNLOAD_URL_HASH "SHA256=433e6660fd1b517d45c8e32e23d3fa5fb122ef84a061a2e9388715f6c21a8dcb" CACHE STRING "re-logging download url hash" FORCE)

# Uncomment/set to proper value if local instead of remote
set(re-logging_ROOT_DIR "") # use remote re-logging
#set(re-logging_ROOT_DIR "${RE_PROJECT_ROOT_DIR}/../../pongasoft/re-logging")

#################
# re-mock
#################
set(re-mock_GIT_REPO "https://github.com/pongasoft/re-mock" CACHE STRING "re-mock git repository url")
set(re-mock_GIT_TAG "v1.8.0" CACHE STRING "re-mock git tag")
set(re-mock_DOWNLOAD_URL "${re-mock_GIT_REPO}/archive/refs/tags/${re-mock_GIT_TAG}.zip" CACHE STRING "re-mock download url" FORCE)
set(re-mock_DOWNLOAD_URL_HASH "SHA256=084a88445b1d3315359c09dd5dbc852801f095b6c1eafdeffc8fc5c88ccbcce4" CACHE STRING "re-mock download url hash" FORCE)

# Uncomment/set to proper value if local instead of remote
set(re-mock_ROOT_DIR "") # use remote re-mock
#set(re-mock_ROOT_DIR "${RE_PROJECT_ROOT_DIR}/../../pongasoft/re-mock")

#################
# re-common
#################
set(re-common_GIT_REPO "https://github.com/pongasoft/re-common" CACHE STRING "re-common git repository url")
set(re-common_GIT_TAG "v3.2.1" CACHE STRING "re-common git tag")
set(re-common_DOWNLOAD_URL "${re-common_GIT_REPO}/archive/refs/tags/${re-common_GIT_TAG}.zip" CACHE STRING "re-common download url" FORCE)
set(re-common_DOWNLOAD_URL_HASH "SHA256=a1d83b0204d12dfb52d1fafe2db7e58659b0b1f4607a0b370271ad3a29ade0cb" CACHE STRING "re-common download url hash" FORCE)

# Uncomment/set to proper value if local instead of remote
set(re-common_ROOT_DIR "") # use remote re-common
#set(re-common_ROOT_DIR "${RE_PROJECT_ROOT_DIR}/../../pongasoft/re-common")

#################
# googletest
#################
set(googletest_GIT_REPO "https://github.com/google/googletest" CACHE STRING "googletest git repository URL")
set(googletest_GIT_TAG "v1.17.0" CACHE STRING "googletest git tag")
set(googletest_DOWNLOAD_URL "${googletest_GIT_REPO}/archive/refs/tags/${googletest_GIT_TAG}.zip" CACHE STRING "googletest download url" FORCE)
set(googletest_DOWNLOAD_URL_HASH "SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff" CACHE STRING "googletest download url hash" FORCE)

# Uncomment/set to proper value if local instead of remote
set(googletest_ROOT_DIR "") # use remote googletest
#set(re-googletest_ROOT_DIR "${RE_PROJECT_ROOT_DIR}/../../google/googletest")

