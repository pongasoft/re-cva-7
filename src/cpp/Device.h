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

#pragma once
#ifndef __pongasoft__BlankDevice__
#define __pongasoft__BlankDevice__

#include "Jukebox.h"
#include "CommonDevice.h"
#include "DeviceState.h"

class Device : public CommonDevice
{
public:
  Device(int iSampleRate);
  virtual ~Device();

  /**
  * @brief	Main starting point for rendering audio
  **/
  virtual void renderBatch(const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount);

private:
  void doInitDevice(TJBox_PropertyDiff const iPropertyDiffs[], TJBox_UInt32 iDiffCount);
  bool doRenderBatch(bool propertyStateChange);

private:
  bool fFirstBatch;
  JBoxPropertyManager fJBoxPropertyManager;
  DeviceState fPreviousDeviceState;
  DeviceState fCurrentDeviceState;

  bool storeCVIn1Value(Motherboard &motherboard, CVPoint const &cvIn1Point);
};

#endif /* defined(__pongasoft__BlankDevice__) */
