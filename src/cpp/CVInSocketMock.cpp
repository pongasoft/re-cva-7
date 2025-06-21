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

#include "CVInSocketMock.h"
#include "ProjectConstants.h"


TJBox_Float64 CVInTestData4Steps::getValue() const
{
  int count = fCount;

  if(fCount >= 4 * MAX_ARRAY_SIZE)
    return MAX_TJbox_Float64;

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
    return MAX_TJbox_Float64;

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
    return MAX_TJbox_Float64;

  return VALUES[fCount / MAX_ARRAY_SIZE];
}
