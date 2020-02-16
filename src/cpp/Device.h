//
//  Device.h
//

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
