#pragma once
#ifndef __pongasoft__ProjectConstants__
#define __pongasoft__ProjectConstants__

#include "Constants.h"
#include "JBoxProperty.h"
#include "CVSocket.h"

const int MAX_ARRAY_SIZE = 256; // size of LCD window
const int MAX_DISPLAY_H = 100; // value in fPropArray will be [0, MAX_DISPLAY_H] for "normal values"
const int HISTORY_SIZE_IN_SECONDS = 10; // how long is the history in seconds
const int MAX_HISTORY_SIZE = 117; // max # of LCD pages (10s history at 192k = HISTORY_SIZE_IN_SECONDS * SR / (kBatchSize * MAX_ARRAY_SIZE))
const int MAX_PROP_UPDATES_PER_BATCH = 32;
const int MIN_HISTORY_OFFSET = 0;
const int MAX_HISTORY_OFFSET = 10000;
const int MAX_INPUT_PAGE_OFFSET = MAX_ARRAY_SIZE - 1;
const int IGNORE_HISTORY_OFFSET_RT = 10001;
const TJBox_Int32 IGNORE_CV_VALUE = 200001;
const int MIN_MAX_AUTO_RESET_OFF = -1;
const TJBox_Float64 MAX_ZOOM_FACTOR_Y = 0.01;

// cv is in the range [-10000, +10000]
const TJBox_Float64 MAX_UNREACHABLE_CV_VALUE = 20000.0;
const TJBox_Float64 MIN_UNREACHABLE_CV_VALUE = -20000.0;

const TJBox_Int32 DISPLAY_NEGATIVE_CLIPPING = 110;
const TJBox_Int32 DISPLAY_POSITIVE_CLIPPING = 120;

inline TJBox_Float64 toValidCVOut(TJBox_Float64 iValue)
{
  if(iValue == MAX_UNREACHABLE_CV_VALUE || iValue == MIN_UNREACHABLE_CV_VALUE)
    return 0;

  return iValue;
}

// Note: because there is only 1 bit used right now, short-circuiting the usage
const TJBox_Int32 CV_IN_STATE_NOT_CONNECTED = 0;
const TJBox_Int32 CV_IN_STATE_CONNECTED = 1;

enum ECVOutType
{
  kCVOutTypePassthrough = 0,
  kCVOutTypeInverse = 1,
  kCVOutTypeUnipolar = 2,
  kCVOutTypeBipolar = 3
};

typedef JBoxProperty<ECVOutType, JBox::toEnum<ECVOutType>, JBox::fromEnum<ECVOutType> > ECVOutTypeJBoxProperty;

inline TJBox_Float64 adjustCVOutValue(ECVOutType iCVOutType, TJBox_Float64 const iValue)
{
  switch(iCVOutType)
  {
    case kCVOutTypePassthrough:
      return iValue;

    case kCVOutTypeInverse:
      return (-iValue);

    case kCVOutTypeUnipolar:
      return (JBox::toUnipolarCV(iValue));

    case kCVOutTypeBipolar:
      return (JBox::toBipolarCV(iValue));

    default:
      throw "not reached";
  }
}

inline TJBox_Int32 computeHistorySize(int iSampleRate)
{
  return static_cast<TJBox_Int32>(round(static_cast<double>(batchCountToFitDuration(iSampleRate, HISTORY_SIZE_IN_SECONDS * 1000)) / MAX_ARRAY_SIZE));
}
#endif /* define(__pongasoft__ProjectConstants__) */
