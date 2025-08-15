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

#include "Device.h"
#include <logging.h>

Device::Device(int iSampleRate): CommonDevice(),
                                 fFirstBatch(true),
                                 fPreviousDeviceState(iSampleRate),
                                 fCurrentDeviceState(iSampleRate)
{
  DLOG_F(INFO, "Device()");
  fCurrentDeviceState.fMotherboard.registerForUpdate(fJBoxPropertyManager);
}

Device::~Device()
{
  // nothing to do
  DLOG_F(INFO, "~Device()");
}

void Device::renderBatch(TJBox_PropertyDiff const iPropertyDiffs[], TJBox_UInt32 iDiffCount)
{
  bool stateChanged = false;

  if(fFirstBatch)
  {
    doInitDevice(iPropertyDiffs, iDiffCount);
    stateChanged = true;
    fFirstBatch = false;
  }
  else
  {
    if(iDiffCount > 0)
    {
      stateChanged |= fJBoxPropertyManager.onUpdate(iPropertyDiffs, iDiffCount);
    }
  }

  stateChanged = fCurrentDeviceState.afterMotherboardUpdate(stateChanged, fPreviousDeviceState);

  stateChanged |= doRenderBatch(stateChanged);

  if(stateChanged)
    fPreviousDeviceState.update(fCurrentDeviceState);

  //fCurrentDeviceState.fMotherboard.fCVIn1.afterRenderBatch();
}

bool Device::doRenderBatch(bool propertyStateChange)
{
  Motherboard &motherboard = fCurrentDeviceState.fMotherboard;

  if(motherboard.fCVIn1.isConnected())
  {
    TJBox_Float64 const cvIn1Value = motherboard.fCVIn1.getValue();

    // simply copy to out if necessary
    for(int i = 0; i < MAX_CV_OUT; i++)
      fCurrentDeviceState.setCVOut(*motherboard.fCVOut[i], cvIn1Value);

    // handle pause/resume
    if(!fCurrentDeviceState.isPaused())
    {
      if(cvIn1Value != MAX_TJbox_Float64)
      {
        CVPoint zoomedCVIn1Point(cvIn1Value);

        // store in the buffer
        fCurrentDeviceState.fCVIn1Buffer.setAt(0, cvIn1Value);

        TJBox_Int32 arrayStart = motherboard.fPropArrayStart.getValue();
        arrayStart++;
        if(arrayStart >= MAX_ARRAY_SIZE)
          arrayStart = 0;

        if(fCurrentDeviceState.zoom(cvIn1Value, zoomedCVIn1Point))
        {
          propertyStateChange |= storeCVIn1Value(motherboard, zoomedCVIn1Point);

          TJBox_Int32 cvIn1Display = fCurrentDeviceState.toDisplayValue(zoomedCVIn1Point.fAvg);
          if(!fCurrentDeviceState.isScreenOff() && fCurrentDeviceState.shouldUpdateArrayStart(cvIn1Display))
          {

            if(fCurrentDeviceState.fPendingUpdates.hasPendingUpdates())
              fCurrentDeviceState.fPendingUpdates.setPendingValue(arrayStart, cvIn1Display);
            else
              propertyStateChange |= motherboard.fPropArray[arrayStart]->storeValueToMotherboardOnUpdate(cvIn1Display);

            propertyStateChange |= motherboard.fPropArrayStart.storeValueToMotherboardOnUpdate(arrayStart);
          }
        }

        fCurrentDeviceState.fCVIn1Buffer.incrementHead();
      }
    }
    else
    {
      // pause mode
      // input page offset has changed or history offset has changed or zoom has changed
      if(fPreviousDeviceState.fMotherboard.fPropInputPageOffset.getValue() != motherboard.fPropInputPageOffset.getValue()
        || fPreviousDeviceState.fMotherboard.fPropInputHistoryOffset.getValue() != fCurrentDeviceState.fMotherboard.fPropInputHistoryOffset.getValue()
        || fPreviousDeviceState.fMotherboard.fPropZoomFactorX.getValue() != motherboard.fPropZoomFactorX.getValue()
        || fPreviousDeviceState.fMotherboard.fPropCVIn1MinMaxReset.getValue() != motherboard.fPropCVIn1MinMaxReset.getValue())
      {
        propertyStateChange |= storeCVIn1Value(motherboard, fCurrentDeviceState.getCVIn1PausedValue());
      }
    }
  }

  propertyStateChange |= fCurrentDeviceState.handlePendingUpdates();

  return propertyStateChange;
}

bool Device::storeCVIn1Value(Motherboard &motherboard, CVPoint const &cvIn1Point)
{
  bool res = false;

  // handle note
  res |= fCurrentDeviceState.storePropCVIn1ValueAsNote(cvIn1Point.fAvg);

  // updated only when reset is not being pressed
  if(motherboard.fPropCVIn1MinMaxReset.getValue() == TJbox_FALSE)
  {
    // handle min
    if(fCurrentDeviceState.fCVIn1MinValue > cvIn1Point.fMin)
    {
      res |= fCurrentDeviceState.setCVIn1MinValue(cvIn1Point.fMin);
    }

    // handle max
    if(fCurrentDeviceState.fCVIn1MaxValue < cvIn1Point.fMax)
    {
      res |= fCurrentDeviceState.setCVIn1MaxValue(cvIn1Point.fMax);
    }
  }

  // store current value
  res |= fCurrentDeviceState.storePropCVIn1Value(cvIn1Point.fAvg);

  return res;
}

void Device::doInitDevice(TJBox_PropertyDiff const iPropertyDiffs[], TJBox_UInt32 iDiffCount)
{
  DLOG_F(INFO, "Initializing device...");

  // initialize properties
  fJBoxPropertyManager.initProperties();

  // processes the updates
  fJBoxPropertyManager.onUpdate(iPropertyDiffs, iDiffCount);

  // initialize current device
  fCurrentDeviceState.init();

  // copy to previous state to initialize it too!
  fPreviousDeviceState.update(fCurrentDeviceState);

  DLOG_F(INFO, "Init complete.");
}
