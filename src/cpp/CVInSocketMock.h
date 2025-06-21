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

#ifndef CVA7_CVInSocketMock_H
#define CVA7_CVInSocketMock_H


#include "JukeboxTypes.h"
#include "JBoxPropertyManager.h"
#include "JBoxProperty.h"
#include "ProjectConstants.h"

class CVInSocketMock : IJBoxPropertyObserver
{
public:
  // to look like a CVInSocket
  CVInSocketMock(char const * iSocketPath):
    iSocketObject(iSocketPath),
    fConnected(iSocketObject, "connected")
  {
  }

private:
  virtual bool update(const TJBox_PropertyDiff &iPropertyDiff) override
  {
    bool res = fConnected.update(iPropertyDiff);

    if(fConnected.getValue())
    {
      JBOX_TRACE("connected => reset");
      reset();
    }

    return res;
  }

  virtual const char *getPropertyPath() const override
  {
    return fConnected.getPropertyPath();
  }

  virtual const TJBox_PropertyRef &getPropertyRef() const override
  {
    return fConnected.fPropertyRef;
  }

  virtual void afterRenderBatch() = 0;

public:
  void registerForUpdate(IJBoxPropertyManager &manager) {
    IJBoxPropertyObserver::registerForUpdate(manager, kJBox_CVInputConnected);
  }

  inline TJBox_Bool isConnected() const { return fConnected.getValue(); }

  virtual TJBox_Float64 getValue() const = 0;
  virtual void reset() = 0;

private:
  JBoxObject iSocketObject;
  BooleanJBoxProperty fConnected;
};

class CVInTestData4Steps : public CVInSocketMock
{
public:
  CVInTestData4Steps(const char *iSocketPath): CVInSocketMock(iSocketPath) { /* do nothing */ }

  virtual void afterRenderBatch() override;
  virtual TJBox_Float64 getValue()const override;
  virtual void reset() override;

private:
  int fCount;
};

class CVInTestDataSteps : public CVInSocketMock
{
public:
  CVInTestDataSteps(const char *iSocketPath);

  virtual void afterRenderBatch() override;
  virtual TJBox_Float64 getValue()const override;
  virtual void reset() override;

private:
  int fCount;
};

class CVInTestDataFullSweep : public CVInSocketMock
{
public:
  CVInTestDataFullSweep(const char *iSocketPath, int iSampleRate):
    CVInSocketMock(iSocketPath),
    fSize(computeHistorySize(iSampleRate) * MAX_ARRAY_SIZE),
    fCount(0)
  {
  }

  virtual void afterRenderBatch() override;
  virtual TJBox_Float64 getValue() const override;
  virtual void reset() override;

private:
  int fSize;
  int fCount;
};

#endif //CVA7_CVInSocketMock_H
