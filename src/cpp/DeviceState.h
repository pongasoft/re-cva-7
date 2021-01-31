//
// Created by Yan Pujante on 5/23/15.
//

#ifndef __pongasoft__BlankDeviceState_h__
#define __pongasoft__BlankDeviceState_h__


#include "JBoxPropertyManager.h"
#include "AudioSocket.h"
#include "Motherboard.h"
#include "CircularBuffer.h"
#include "ProjectConstants.h"
#include "ZoomWindow.h"

int const NO_UPDATE = -1;

template<int size>
class PendingUpdates
{
public:
  PendingUpdates(Int32JBoxProperty **iPropArray) : fPendingCount(0), fPropArray(iPropArray)
  {
    for(int i = 0; i < size; ++i)
    {
      fPendingUpdates[i] = NO_UPDATE;
    }
  }

  inline bool hasPendingUpdates() const { return fPendingCount > 0; }

  inline TJBox_Int32 getValue(int i) const
  {
    JBOX_ASSERT_RANGE(i, 0, size - 1);

    TJBox_Int32 value = fPendingUpdates[i];

    if(value == NO_UPDATE)
      value = fPropArray[i]->getValue();

    return value;
  }

  inline void setPendingValue(int i, TJBox_Int32 iValue) {
    JBOX_ASSERT_RANGE(i, 0, size - 1);

    if(fPendingUpdates[i] == NO_UPDATE)
    {
      fPendingCount++;
    }
    fPendingUpdates[i] = iValue;
  }

  bool handlePendingUpdates(int maxUpdates)
  {
    bool res = false;

    if(hasPendingUpdates())
    {
      int max = maxUpdates;
      for(int i = MAX_ARRAY_SIZE - 1; i >=0 && max > 0; --i)
      {
        if(storeValueToMotherboard(i))
        {
          res = true;
          max--;
        }
      }
    }

    return res;
  }

private:
  inline bool storeValueToMotherboard(int i)
  {
    JBOX_ASSERT_RANGE(i, 0, size - 1);

    bool res = false;

    TJBox_Int32 value = fPendingUpdates[i];

    if(value != NO_UPDATE)
    {
      res |= fPropArray[i]->storeValueToMotherboardOnUpdate(value);
      fPendingUpdates[i] = NO_UPDATE;
      fPendingCount--;

      JBOX_ASSERT(fPendingCount >= 0);
    }

    return res;
  }

private:
  int fPendingCount;
  TJBox_Int32 fPendingUpdates[size];
  Int32JBoxProperty **fPropArray;
};

class DeviceState : ZoomWindow::IZoomCallback
{
public:
  DeviceState(int iSampleRate);
  ~DeviceState() {}

 /*
  * Called to initialize the state on first batch
  */
  void init();

  void handleDisconnect();

  bool afterMotherboardUpdate(bool motherboardStateChanged,
                              DeviceState const &previousState);

  bool handlePendingUpdates() { return fPendingUpdates.handlePendingUpdates(MAX_PROP_UPDATES_PER_BATCH); }

  void update(const DeviceState &rhs);

  inline bool resetCVIn1MinMaxValues()
  {
    bool res = false;

    res |= setCVIn1MinValue(MAX_UNREACHABLE_CV_VALUE);
    res |= setCVIn1MaxValue(MIN_UNREACHABLE_CV_VALUE);

    fAutoResetFrameCount = fMotherboard.fPropCVIn1MinMaxAutoReset.getAutoResetFrameCount();

    return res;
  }

  inline bool setCVIn1MinValue(TJBox_Float64 iValue)
  {
    fCVIn1MinValue = iValue;
    return storePropCVIn1MinValue();
  }

  inline bool storePropCVIn1MinValue()
  {
    bool res = fMotherboard.fPropCVIn1MinValue.storeValueToMotherboardOnUpdate(fCVIn1MinValue);
    if(fMotherboard.fCVOutMinValue1.isConnected())
      fMotherboard.fCVOutMinValue1.storeValueToMotherboardOnUpdate(toValidCVOut(fCVIn1MinValue));
    return res;
  }

  inline bool setCVIn1MaxValue(TJBox_Float64 iValue)
  {
    fCVIn1MaxValue = iValue;
    return storePropCVIn1MaxValue();
  }

  inline bool storePropCVIn1MaxValue()
  {
    bool res = fMotherboard.fPropCVIn1MaxValue.storeValueToMotherboardOnUpdate(fCVIn1MaxValue);
    if(fMotherboard.fCVOutMaxValue1.isConnected())
      fMotherboard.fCVOutMaxValue1.storeValueToMotherboardOnUpdate(toValidCVOut(fCVIn1MaxValue));
    return res;
  }

  inline bool storePropCVIn1Value(TJBox_Float64 iValue)
  {
    bool res = fMotherboard.fPropCVIn1Value.storeValueToMotherboardOnUpdate(iValue);
    fMotherboard.fCVOutValue1.storeValueToMotherboardOnUpdate(toValidCVOut(iValue));
    return res;
  }

  inline bool storePropCVIn1ValueAsNote(TJBox_Float64 iValue)
  {
    fLastCVIn1ValueAsNote = iValue;

    bool res = false;

    if(!isMidiOff())
      res = fMotherboard.fPropCVIn1ValueAsNote.storeValueToMotherboardOnUpdate(JBox::toNote(iValue));

    return res;
  }

  inline TJBox_Int32 toDisplayValue(TJBox_Float64 iRawValue)
  {
    iRawValue *= fZoomFactorY;

    if(equals5DP(iRawValue, -1.0))
      iRawValue = -1.0;

    if(equals5DP(iRawValue, 1.0))
      iRawValue = 1.0;

    if(iRawValue < -1.0)
      return DISPLAY_NEGATIVE_CLIPPING;

    if(iRawValue > 1.0)
      return DISPLAY_POSITIVE_CLIPPING;

    return static_cast<TJBox_Int32>(round((iRawValue + 1.0) / 2.0 * MAX_DISPLAY_H));
  }

  inline bool storeCVIn1State()
  {
    if(fMotherboard.fCVIn1.isConnected())
      return fMotherboard.fPropCVIn1State.storeValueToMotherboardOnUpdate(CV_IN_STATE_CONNECTED);
    else
      return fMotherboard.fPropCVIn1State.storeValueToMotherboardOnUpdate(CV_IN_STATE_NOT_CONNECTED);
  }

  inline bool isPaused() const
  {
    return fMotherboard.fPropInputPaused.getValue();
  }

  inline bool handleCVOutChange(CVOut const &fPreviousCVOut, CVOut &fCVOut) const
  {
    bool res = false;

    // if cv out is connected
    if(fCVOut.fCVOutSocket.isConnected())
    {
      // it was not previously connected or type has changed => need to adjust new cv out!
      if(!fPreviousCVOut.fCVOutSocket.isConnected() ||
        fPreviousCVOut.fPropCVOutType.getValue() != fCVOut.fPropCVOutType.getValue())
      {
        if(fMotherboard.fCVIn1.isConnected())
          res |= fCVOut.fCVOutSocket.storeValueToMotherboardOnUpdate(adjustCVOutValue(fCVOut.fPropCVOutType.getValue(),
                                                                                      fMotherboard.fCVIn1.getValue()));
        else
          res |= fCVOut.fCVOutSocket.storeValueToMotherboardOnUpdate(0);
      }
    }

    return res;
  }

  inline void setCVOut(CVOut &fCVOut, TJBox_Float64 const iValue)
  {
    if(fCVOut.fCVOutSocket.isConnected())
    {
      fCVOut.fCVOutSocket.storeValueToMotherboardOnUpdate(adjustCVOutValue(fCVOut.fPropCVOutType.getValue(),
                                                                           iValue));
    }
  }

  bool recomputeArray();

  CVPoint getCVIn1PausedValue();

  /**
   * If the same value is constantly being displayed there is no point in updating the display...
   */
  inline bool shouldUpdateArrayStart(TJBox_Int32 iCVIn1Display)
  {
    if(iCVIn1Display == fLastCVIn1Display)
    {
      if(fCVIn1DisplayUniqueCount > MAX_ARRAY_SIZE)
        return false;

      fCVIn1DisplayUniqueCount++;
    }
    else
    {
      fCVIn1DisplayUniqueCount = 0;
    }

    fLastCVIn1Display = iCVIn1Display;


    return true;
  }

  inline bool zoom(TJBox_Float64 const iCVValue, CVPoint &oCVValue)
  {
    return fZoomWindow.nextZoomedValue(iCVValue, oCVValue);
  }

  inline bool isScreenOff() const { return fMotherboard.fPropScreenOn.getValue() == TJbox_FALSE; }

  inline bool isMidiOff() const { return fMotherboard.fPropMidiOn.getValue() == TJbox_FALSE; }

public:
  Motherboard fMotherboard;

  PendingUpdates<MAX_ARRAY_SIZE> fPendingUpdates;
  TJBox_Int32 const fHistorySize;
  CircularBuffer<TJBox_Float64> fCVIn1Buffer;
  TJBox_Float64 fCVIn1MinValue;
  TJBox_Float64 fCVIn1MaxValue;

  // count (down) until when to reset min/max
  int fAutoResetFrameCount;

  // no need to compute over and over
  TJBox_Float64 fZoomFactorY;

private:
  void onZoomFactorXChange();
  void onHistoryOffsetChange();

  ZoomWindow fZoomWindow;

  // used for optimizing display... if the screen stays static there is no need to increment the buffer
  TJBox_Int32 fLastCVIn1Display;
  int fCVIn1DisplayUniqueCount;

  // used to optimize keyboard display
  TJBox_Float64 fLastCVIn1ValueAsNote;

  virtual bool zoomValue(int iIdx, CVPoint const &iValue) override;

  void onZoomFactorYChange();
};


#endif //__pongasoft__BlankDeviceState_h__
