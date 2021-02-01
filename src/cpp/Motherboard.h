//
// Created by Yan Pujante on 7/15/15.
//


#ifndef __Motherboard_H_
#define __Motherboard_H_


#include "JBoxPropertyManager.h"
#include "JBoxProperty.h"
#include "CVSocket.h"
#include "ProjectConstants.h"
#include "jbox.h"

#ifndef  __phdsp__
#include "CVInSocketMock.h"
#endif // !__phdsp__

const TJBox_Tag PROP_CV_IN_1_MIN_MAX_RESET_TAG = 3700;
const TJBox_Tag PROP_INPUT_PAUSED_TAG = 3900;
const TJBox_Tag PROP_INPUT_PAGE_OFFSET_TAG = 4100;
const TJBox_Tag PROP_INPUT_HISTORY_OFFSET_TAG = 4200;
const TJBox_Tag PROP_ZOOM_FACTOR_X_TAG = 4400;
const TJBox_Tag PROP_ZOOM_FACTOR_Y_TAG = 4450;
const TJBox_Tag PROP_CV_IN_1_MIN_MAX_AUTO_RESET_TAG = 4500;
const TJBox_Tag PROP_SCREEN_ON_TAG = 4800;
const TJBox_Tag PROP_MIDI_ON_TAG = 4900;
const TJBox_Tag PROP_CV_OUT_1_TYPE_TAG = 4700;
const TJBox_Tag PROP_CV_OUT_2_TYPE_TAG = 4701;
const TJBox_Tag PROP_CV_OUT_3_TYPE_TAG = 4702;
const TJBox_Tag PROP_CV_OUT_4_TYPE_TAG = 4703;

class MinMaxAutoResetJBoxProperty : public JBoxPropertyUpdateListener<TJBox_Int32>
{
public:
  MinMaxAutoResetJBoxProperty(char const *iPropertyPath, int iSampleRate):
    fPropCVIn1MinMaxAutoReset(iPropertyPath),
    fSampleRate(iSampleRate),
    fAutoResetFrameCount(MIN_MAX_AUTO_RESET_OFF)
  {
    fPropCVIn1MinMaxAutoReset.setUpdateListener(this);
  }

  inline int getAutoResetFrameCount() const { return fAutoResetFrameCount; }

  inline void registerForUpdate(IJBoxPropertyManager &manager, TJBox_Tag iTag) { fPropCVIn1MinMaxAutoReset.registerForUpdate(manager, iTag); }

  void onPropertyUpdated(const TJBox_Int32 &previousValue, const TJBox_Int32 &newValue) override;


public:
  Int32JBoxProperty fPropCVIn1MinMaxAutoReset;

private:
  int fSampleRate;
  int fAutoResetFrameCount;
};

class CVOut
{
public:
  explicit CVOut(int iIndex) :
    fCVOutSocket{jbox::PropertyName::printf("cv_out_%d", iIndex)},
    fPropCVOutType{jbox::custom_property("prop_cv_out_%d_type", iIndex)}
  {}

  CVOutSocket fCVOutSocket;
  ECVOutTypeJBoxProperty fPropCVOutType;

  void registerForUpdate(IJBoxPropertyManager &iManager, TJBox_Tag iCVOutTypeTag)
  {
    fCVOutSocket.registerForUpdate(iManager);
    fPropCVOutType.registerForUpdate(iManager, iCVOutTypeTag);
  }
};

class CVInValue
{
public:
  CVInValue(char const *iCVInValueIntPart, char const *iCVInValueDecimalPart) :
    fPropCVInValueIntPart(iCVInValueIntPart), fPropCVInValueDecimalPart(iCVInValueDecimalPart) {};

  void registerForInit(IJBoxPropertyManager &manager)
  {
    fPropCVInValueIntPart.registerForInit(manager);
    fPropCVInValueDecimalPart.registerForInit(manager);
  }

  inline bool storeValueToMotherboardOnUpdate(TJBox_Float64 const iCVValue)
  {
    bool res = false;

    if(iCVValue == MAX_UNREACHABLE_CV_VALUE || iCVValue == MIN_UNREACHABLE_CV_VALUE)
    {
      res |= fPropCVInValueIntPart.storeValueToMotherboardOnUpdate(0);
      res |= fPropCVInValueDecimalPart.storeValueToMotherboardOnUpdate(IGNORE_CV_VALUE);
    }
    else
    {
      if(fabs(iCVValue) <= 1.0)
      {
        // no integer part
        res |= fPropCVInValueIntPart.storeValueToMotherboardOnUpdate(0);

        // encode the sign with the decimal part (5 digits of precision)
        TJBox_Int32 value = static_cast<TJBox_Int32>(round((iCVValue + 1.0) * 100000));
        JBOX_ASSERT_RANGE(value, 0, 200000);
        res |= fPropCVInValueDecimalPart.storeValueToMotherboardOnUpdate(value);
      }
      else
      {
        // because it's cv value, intPart is in range [-10000, 10000]
        TJBox_Int32 intPart = (TJBox_Int32) iCVValue;
        TJBox_Float64 decimalPart = iCVValue - intPart;

        intPart += 10001; // to be in range 1-20001 (encodes the sign)
        JBOX_ASSERT_RANGE(intPart, 1, 20001);
        res |= fPropCVInValueIntPart.storeValueToMotherboardOnUpdate(intPart);

        // does not encode the sign
        TJBox_Int32 decimalPartAsInt = (TJBox_Int32) (round(fabs(decimalPart) * 100000));
        JBOX_ASSERT_RANGE(decimalPartAsInt, 0, 100000);
        res |= fPropCVInValueDecimalPart.storeValueToMotherboardOnUpdate(decimalPartAsInt);
      }
    }

    return res;
  }

  Int32JBoxProperty fPropCVInValueIntPart;
  Int32JBoxProperty fPropCVInValueDecimalPart;
};

class Motherboard
{
public:
  Motherboard(int iSampleRate);

  void registerForUpdate(IJBoxPropertyManager &manager);
  void update(Motherboard const &rhs);

public:
  CVInSocket fCVIn1;
  //CVInTestDataSteps fCVIn1;
//  CVInTestDataFullSweep fCVIn1;
//  CVInTestData4Steps fCVIn1;
  CVOut *fCVOut[MAX_CV_OUT];

  CVOutSocket fCVOutValue1;
  CVOutSocket fCVOutMinValue1;
  CVOutSocket fCVOutMaxValue1;

  CVInValue fPropCVIn1Value;
  Int32JBoxProperty fPropCVIn1State;
  Int32JBoxProperty fPropCVIn1ValueAsNote;
  CVInValue fPropCVIn1MinValue;
  CVInValue fPropCVIn1MaxValue;
  BooleanJBoxProperty fPropCVIn1MinMaxReset;
  BooleanJBoxProperty fPropInputPaused;
  Int32JBoxProperty fPropInputPageOffset;
  Int32JBoxProperty fPropInputHistoryOffset;
  Int32JBoxProperty fPropInputHistoryOffsetRT;
  Float64JBoxProperty fPropZoomFactorX;
  Float64JBoxProperty fPropZoomFactorY;
  Int32JBoxProperty fPropPlus05Axis;
  MinMaxAutoResetJBoxProperty fPropCVIn1MinMaxAutoReset;
  BooleanJBoxProperty fPropScreenOn;
  BooleanJBoxProperty fPropMidiOn;

  Int32JBoxProperty fPropArrayStart;
  Int32JBoxProperty *fPropArray[MAX_ARRAY_SIZE];
};


#endif //__Motherboard_H_
