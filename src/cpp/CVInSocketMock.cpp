//
// Created by Yan Pujante on 9/17/16.
//

#include "CVInSocketMock.h"
#include "ProjectConstants.h"


TJBox_Float64 CVInTestData4Steps::getValue() const
{
  int count = fCount;

  if(fCount >= 4 * MAX_ARRAY_SIZE)
    return MAXFLOAT;

  if(count < MAX_ARRAY_SIZE)
    return 0;

  if(count < 2 * MAX_ARRAY_SIZE)
    return 0.25;

  if(count < 3 * MAX_ARRAY_SIZE)
    return 0.5;

  return 0.75;

}

void CVInTestData4Steps::reset()
{
  fCount = 0;
}

void CVInTestData4Steps::afterRenderBatch()
{
  if(fCount < 4 * MAX_ARRAY_SIZE)
    fCount++;
}

TJBox_Float64 CVInTestDataFullSweep::getValue() const
{
  if(fCount >= fSize)
    return MAXFLOAT;

  return 2.0 / (fSize - 1) * fCount - 1;
}

void CVInTestDataFullSweep::reset()
{
  fCount = 0;
}

void CVInTestDataFullSweep::afterRenderBatch()
{
  if(fCount < fSize)
    fCount++;
}

//static const TJBox_Float64 VALUES[] = {
//-0.5
//};

static const TJBox_Float64 VALUES[] = {
  -10000,
  -1234.56789,
  -123.456789,
  -12.3456789,
  -5.0,
  -4.56789,
  -2.0,
  -1.0,
  -0.5,
  0,
  0.5,
  1.0,
  2.0,
  4.56789,
  5,
  12.3456789,
  123.456789,
  1234.56789,
  10000
};


static const int VALUES_SIZE = sizeof(VALUES) / sizeof(TJBox_Float64);

CVInTestDataSteps::CVInTestDataSteps(const char *iSocketPath) :
  CVInSocketMock(iSocketPath), fCount(0)
{
}

void CVInTestDataSteps::afterRenderBatch()
{
  if(fCount < MAX_ARRAY_SIZE * VALUES_SIZE)
    fCount++;
}

void CVInTestDataSteps::reset()
{
  fCount = 0;
}

TJBox_Float64 CVInTestDataSteps::getValue() const
{
  if(fCount >= MAX_ARRAY_SIZE * VALUES_SIZE)
    return MAXFLOAT;

  return VALUES[fCount / MAX_ARRAY_SIZE];
}
