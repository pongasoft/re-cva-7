/*
 * Copyright (c) 2025 pongasoft
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * @author Yan Pujante
 */

#include <gtest/gtest.h>
#include <Device.h>
#include <re/mock/DeviceTesters.h>
#include <re_cmake_build.h>

using namespace re::mock;

/**
 * The device has 2 mono out sockets but we still use the InstrumentTester and wire Out to Left and Aux to Right
 * for simplicity of writing tests.
 */

// Device - Init
TEST(Device, Init)
{
  RE_LOGGING_INIT_FOR_TEST("Device.Init");

  auto c = DeviceConfig<Device>::fromJBoxExport(RE_CMAKE_PROJECT_DIR);
  auto tester = HelperTester<Device>(c);

  tester.nextBatch();
}
