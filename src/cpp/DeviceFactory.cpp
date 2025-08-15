/*
 * Copyright (c) 2015-2025 pongasoft
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

#include "JukeboxExports.h"
#include "Device.h"
#include <logging.h>

CommonDevice *createDevice(const TJBox_Value iParams[], TJBox_UInt32 iCount)
{
  DCHECK_F(iCount >= 1);

  RE_LOGGING_INIT_FOR_RE("CVA7");

  TJBox_Float64 sampleRate = JBox_GetNumber(iParams[0]);
  int sampleRateInt = static_cast<int>(sampleRate);

  DLOG_F(INFO, "New instance with sample rate => %d", sampleRateInt);

  return new Device(sampleRateInt);
}
