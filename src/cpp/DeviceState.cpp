//
// Created by Yan Pujante on 5/23/15.
//

#include "DeviceState.h"
#include "Utils.h"

DeviceState::DeviceState(int iSampleRate) :
  fMotherboard(iSampleRate),
  fPendingUpdates(fMotherboard.fPropArray),
  fHistorySize(computeHistorySize(iSampleRate)),
  fCVIn1Buffer(fHistorySize * MAX_ARRAY_SIZE),
  fAutoResetFrameCount(MIN_MAX_AUTO_RESET_OFF),
  fZoomFactorY(1.0),
  fZoomWindow(MAX_ARRAY_SIZE, fCVIn1Buffer),
  fLastCVIn1Display(0),
  fCVIn1DisplayUniqueCount(0),
  fLastCVIn1ValueAsNote(0)
{
  JBOX_ASSERT(fHistorySize <= MAX_HISTORY_SIZE);
}

void DeviceState::init()
{
  fCVIn1Buffer.init(0.0);

  // initializes to 0 (center of screen)
  TJBox_Int32 initValue = toDisplayValue(0);
  for(int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    fMotherboard.fPropArray[i]->initMotherboard(initValue);
  }

  // since the device starts empty (fMaxSampleValuePerFrame) there is no point in recovering undo values
  fMotherboard.fPropArrayStart.initMotherboard(0);

  storePropCVIn1Value(MAX_UNREACHABLE_CV_VALUE);

  fMotherboard.fPropPlus05Axis.initMotherboard(toDisplayValue(0.5));

  fLastCVIn1Display = 0;
  fCVIn1DisplayUniqueCount = 0;
  fLastCVIn1ValueAsNote = 0;

  resetCVIn1MinMaxValues();

  // initializes zoom factor
  onZoomFactorXChange();

  // initialize scale
  onZoomFactorYChange();
}

void DeviceState::handleDisconnect()
{
  fCVIn1Buffer.init(0.0);

  for(int i = 0; i < MAX_ARRAY_SIZE; i++)
  {
    fPendingUpdates.setPendingValue(i, 0);
  }

  fMotherboard.fCVOut1.fCVOutSocket.storeValueToMotherboardOnUpdate(0);
  fMotherboard.fCVOut2.fCVOutSocket.storeValueToMotherboardOnUpdate(0);
  fMotherboard.fCVOut3.fCVOutSocket.storeValueToMotherboardOnUpdate(0);
  fMotherboard.fCVOut4.fCVOutSocket.storeValueToMotherboardOnUpdate(0);
  storePropCVIn1Value(MAX_UNREACHABLE_CV_VALUE);

  fLastCVIn1Display = 0;
  fCVIn1DisplayUniqueCount = 0;

  resetCVIn1MinMaxValues();
}

bool DeviceState::afterMotherboardUpdate(bool motherboardStateChanged, DeviceState const &previousState)
{
  // disconnected
  Motherboard const &previousMotherboard = previousState.fMotherboard;

  bool needToRecomputeArray = false;

  if(previousMotherboard.fCVIn1.isConnected() && !fMotherboard.fCVIn1.isConnected())
  {
    handleDisconnect();
    needToRecomputeArray = true;
    motherboardStateChanged = true;
  }

  // handle potential change with cv out
  motherboardStateChanged |= handleCVOutChange(previousMotherboard.fCVOut1, fMotherboard.fCVOut1);
  motherboardStateChanged |= handleCVOutChange(previousMotherboard.fCVOut2, fMotherboard.fCVOut1);
  motherboardStateChanged |= handleCVOutChange(previousMotherboard.fCVOut3, fMotherboard.fCVOut1);
  motherboardStateChanged |= handleCVOutChange(previousMotherboard.fCVOut4, fMotherboard.fCVOut1);

  // recomputing the window no matter what when pause mode changes
  if(previousState.isPaused() != isPaused())
  {
    needToRecomputeArray = true;
  }

  // turning the screen back on should recompute the window
  if(previousState.isScreenOff() && !isScreenOff())
  {
    needToRecomputeArray = true;
  }

  // when midi is turned back on, we need to make sure the latest value as note is current
  if(previousState.isMidiOff() && !isMidiOff())
  {
    motherboardStateChanged |= storePropCVIn1ValueAsNote(fLastCVIn1ValueAsNote);
  }

  // resetting min/max for CVIn1
  if(fMotherboard.fPropCVIn1MinMaxReset.getValue() == TJbox_TRUE)
  {
    motherboardStateChanged |= resetCVIn1MinMaxValues();
  }
  else
  {
    if(fMotherboard.fPropCVIn1MinMaxAutoReset.getAutoResetFrameCount() !=
      previousMotherboard.fPropCVIn1MinMaxAutoReset.getAutoResetFrameCount())
    {
      fAutoResetFrameCount = fMotherboard.fPropCVIn1MinMaxAutoReset.getAutoResetFrameCount();
    }
    else
    {
      if(!isPaused() && fAutoResetFrameCount != MIN_MAX_AUTO_RESET_OFF)
      {
        fAutoResetFrameCount--;

        if(fAutoResetFrameCount == 0)
        {
          motherboardStateChanged |= resetCVIn1MinMaxValues();
        }
      }
    }
  }

  // making sure that the cv sockets are up to date
  if(fMotherboard.fCVOutMinValue1.isNewlyConnected(previousMotherboard.fCVOutMinValue1))
    storePropCVIn1MinValue();

  if(fMotherboard.fCVOutMaxValue1.isNewlyConnected(previousMotherboard.fCVOutMaxValue1))
    storePropCVIn1MaxValue();

  // zoom factor change...
  if(previousMotherboard.fPropZoomFactorX.getValue() != fMotherboard.fPropZoomFactorX.getValue())
  {
    onZoomFactorXChange();
    needToRecomputeArray = true;
  }

  // zoom factor change Y...
  if(previousMotherboard.fPropZoomFactorY.getValue() != fMotherboard.fPropZoomFactorY.getValue())
  {
    onZoomFactorYChange();
    needToRecomputeArray = true;
  }

  // make sure that CVIn1State reflects the actual state
  motherboardStateChanged |= storeCVIn1State();

  // only check those in pause mode...
  if(isPaused())
  {
    // history offset has changed
    if(previousMotherboard.fPropInputHistoryOffset.getValue() != fMotherboard.fPropInputHistoryOffset.getValue())
    {
      onHistoryOffsetChange();
      needToRecomputeArray = true;
    }
  }
  else
  {
    fMotherboard.fPropInputHistoryOffsetRT.storeValueToMotherboardOnUpdate(MAX_HISTORY_OFFSET);

    // when un-pausing, the history gets reset at the beginning
    if(previousState.isPaused())
    {
      // resetting RT usage
      fZoomWindow.setWindowOffset(MAX_HISTORY_OFFSET);
      needToRecomputeArray = true;
    }
  }

  // recompute array if necessary
  if(needToRecomputeArray)
    motherboardStateChanged |= recomputeArray();

  return motherboardStateChanged;
}

void DeviceState::onHistoryOffsetChange()
{
  fZoomWindow.setWindowOffset(fMotherboard.fPropInputHistoryOffset.getValue());
  // resetting RT usage
  fMotherboard.fPropInputHistoryOffsetRT.storeValueToMotherboardOnUpdate(IGNORE_HISTORY_OFFSET_RT);
}

void DeviceState::update(const DeviceState &rhs)
{
  fMotherboard.update(rhs.fMotherboard);
}

bool DeviceState::zoomValue(int iIdx, CVPoint const &iValue)
{
  fPendingUpdates.setPendingValue(iIdx, toDisplayValue(iValue.fAvg));
  return false;
}

bool DeviceState::recomputeArray()
{
  if(isScreenOff())
    return false;

  bool res = fZoomWindow.computeZoomWindow(*this);
  res |= fMotherboard.fPropArrayStart.storeValueToMotherboardOnUpdate(MAX_ARRAY_SIZE - 1);
  fCVIn1DisplayUniqueCount = 0;
  fLastCVIn1Display = fPendingUpdates.getValue(MAX_ARRAY_SIZE - 1);
  return res;
}

CVPoint DeviceState::getCVIn1PausedValue()
{
  return fZoomWindow.computeZoomValue(fMotherboard.fPropInputPageOffset.getValue());
}

void DeviceState::onZoomFactorXChange()
{

//  TJBox_Float64 zoomFactorX = fMotherboard.fPropZoomFactorX.getValue();
//  if(zoomFactorX >= 0.75)
//  {
//    zoomFactorX = 1.0;
//  }
//  else
//  {
//    if(zoomFactorX >= 0.5)
//    {
//      zoomFactorX = 2.0;
//    }
//    else
//    {
//      if(zoomFactorX >= 0.25)
//      {
//        zoomFactorX = 3.0;
//      }
//      else
//      {
//        zoomFactorX = 4.0;
//      }
//    }
//  }

  TJBox_Int32 inputPageOffset = isPaused() ? fMotherboard.fPropInputPageOffset.getValue() : MAX_INPUT_PAGE_OFFSET;
  int inputHistoryOffset = fZoomWindow.setZoomFactor(fMotherboard.fPropZoomFactorX.getValue(),
                                                     inputPageOffset);

  if(fMotherboard.fPropInputHistoryOffset.getValue() != inputHistoryOffset)
    fMotherboard.fPropInputHistoryOffsetRT.storeValueToMotherboardOnUpdate(inputHistoryOffset);
  else
    fMotherboard.fPropInputHistoryOffsetRT.storeValueToMotherboardOnUpdate(IGNORE_HISTORY_OFFSET_RT);
}

void DeviceState::onZoomFactorYChange()
{
  fZoomFactorY = Utils::Lerp(MAX_ZOOM_FACTOR_Y, 1.0).compute(fMotherboard.fPropZoomFactorY.getValue());
  fMotherboard.fPropPlus05Axis.storeValueToMotherboardOnUpdate(toDisplayValue(0.5));
}

