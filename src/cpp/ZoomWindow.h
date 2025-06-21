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

#ifndef CVA7_ZOOMWINDOW_H
#define CVA7_ZOOMWINDOW_H

#include "Constants.h"
#include "Jukebox.h"
#include "CircularBuffer.h"
#include "ProjectConstants.h"

const int FLOAT_TO_INT_FACTOR = 10;

const int MAX_ZOOM_POINTS = 10; // computed in zoom.groovy

struct CVPoint
{
  CVPoint() : fAvg(0), fMin(0), fMax(0) {}
  CVPoint(TJBox_Float64 iValue) : fAvg(iValue), fMin(iValue), fMax(iValue) {}

  TJBox_Float64 fAvg;
  TJBox_Float64 fMin;
  TJBox_Float64 fMax;
};

class ZoomAlgorithm
{
public:
  inline void init(int iZoomFactor)
  {
    fZoomFactor = iZoomFactor;
  }

  inline void start(TJBox_Float64 iSample, int iPercentFirstSample)
  {
    fCVPoint.fAvg = iSample * iPercentFirstSample / FLOAT_TO_INT_FACTOR;
    fCVPoint.fMin = iSample;
    fCVPoint.fMax = iSample;
  }

  inline void next(TJBox_Float64 iSample)
  {
    fCVPoint.fAvg += iSample;
    fCVPoint.fMin = fmin(fCVPoint.fMin, iSample);
    fCVPoint.fMax = fmax(fCVPoint.fMax, iSample);
  }

  inline CVPoint end(TJBox_Float64 iSample, int iPercentLastSample)
  {
    fCVPoint.fAvg += iSample * iPercentLastSample / FLOAT_TO_INT_FACTOR;
    fCVPoint.fAvg /= ((TJBox_Float64) (fZoomFactor)) / FLOAT_TO_INT_FACTOR;
    fCVPoint.fMin = fmin(fCVPoint.fMin, iSample);
    fCVPoint.fMax = fmax(fCVPoint.fMax, iSample);

    return fCVPoint;
  }

private:
  CVPoint fCVPoint;
  int fZoomFactor;
};

/**
 * Maintains the current state of zoom */
class Zoom
{
public:
  Zoom();
  Zoom(TJBox_Float64 iZoomFactor);

  /**
   * @param iZoomFactor zoom factor is 1.0 for min zoom. 2.0 for example means twice as big... etc...
   */
  void setZoomFactor(TJBox_Float64 iZoomFactor);

  inline bool isNoZoom() const { return fZoomFactor == FLOAT_TO_INT_FACTOR; }

  bool nextZoomedValue(TJBox_Float64 iSample, CVPoint &oNextZoomedValue);

  void reset();

private:
  friend class ZoomWindow;

  void reset(TJBox_Float64 iSample, int iPercent);

  int fZoomFactor;
  ZoomAlgorithm fZoomAlgorithm;
  int fLastPercent;
};

/**
 * Represents a zoom window
 */
class ZoomWindow
{
public:
  class IZoomCallback
  {
  public:
    virtual bool zoomValue(int iIdx, CVPoint const &iValue) = 0;
  };

public:
  ZoomWindow(int iVisibleWindowSize, CircularBuffer<TJBox_Float64> const &iBuffer);

  /**
   * @param iZoomFactorPercent zoom factor between 0-1 (where 1 is min zoom, and 0 is max zoom)
   */
  void setZoomFactor(TJBox_Float64 iZoomFactorPercent);

  /**
   * change the zoom factor while making sure it zooms "around" iInputPageOffset
   * @return the new iInputHistoryOffset
   */
  int setZoomFactor(TJBox_Float64 iZoomFactorPercent, int iInputPageOffset);

  inline bool nextZoomedValue(TJBox_Float64 iSample, CVPoint &oNextZoomedValue) {
    return fZoom.nextZoomedValue(iSample, oNextZoomedValue);
  };

  CVPoint computeZoomValue(int iInputPageOffset) const;

  bool computeZoomWindow(IZoomCallback &callback);

  void setWindowOffset(int iInputHistoryOffset);

private:
  /**
   * A single point in the ZoomWindow
   */
  struct ZoomPoint
  {
    inline ZoomPoint() : fPercentFirstSample(FLOAT_TO_INT_FACTOR), fBufferOffset(-1) {}

    // the percentage of the first sample (using FLOAT_TO_INT_FACTOR as 100%)
    int fPercentFirstSample;

    // the offset where this zoom point starts in the underlying buffer (fBuffer)
    int fBufferOffset;
  };

private:
  // here iIdx is relative to the right of the screen (see fWindowOffset)
  ZoomPoint getZoomPoint(int iIdx) const;

  // Convenient method to compute the zoom point at the left of the LCD screen
  ZoomPoint getZoomPointLeftOfScreen() const;

  /**
   * Computes the zoom value from a zoom point
   */
  CVPoint computeZoomValue(ZoomPoint const &iZoomPoint) const;

  /**
   * Find the zoom point so that zp.fBufferOffset is the closest to iBufferOffset and returns
   * the index. Also tries to find the point where the zoom value would be the closest.
   */
  int findClosestWindowIndex(int iBufferOffset, CVPoint const &iZoomValue) const;

  inline int minWindowIdx() const { return fMinWindowOffset - fVisibleWindowSize + 1; }

private:
  int const fVisibleWindowSize;

  // Since points repeat, there is no need to keep more (max repeat = 10 points!)
  ZoomPoint fPoints[MAX_ZOOM_POINTS];

  // how many samples does fPoints represents
  int fPointsSizeInSamples;

  // offset in the zoomed window (with -1 being the right of the LCD screen for most recent point)
  // this assumes that it corresponds to -1 in fBuffer as well
  int fWindowOffset;

  // the minimum window offset to fit inside the buffer
  int fMinWindowOffset;

  // the maximum amount of zoom allowed
  int fMaxZoomFactor;

  // the underlying buffer
  CircularBuffer<TJBox_Float64> const & fBuffer;

  /**
   * Zoom associated to this window */
  Zoom fZoom;
};

#endif //CVA7_ZOOMWINDOW_H
