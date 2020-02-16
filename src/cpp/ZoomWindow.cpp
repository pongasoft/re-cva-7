
#ifndef  __phdsp__
#include <string>
#endif // !__phdsp__
#include "JukeboxTypes.h"
#include "ZoomWindow.h"
#include "ProjectConstants.h"

int const MAX_WINDOW_OFFSET = -1;

int toIntZoomFactor(TJBox_Float64 iZoomFactor);

int toIntZoomFactor(TJBox_Float64 iZoomFactor)
{
  JBOX_ASSERT(iZoomFactor >= 1.0);
  return static_cast<int>(iZoomFactor * FLOAT_TO_INT_FACTOR);
}

////////////////////////////////////////////////////////////
// Zoom
////////////////////////////////////////////////////////////
Zoom::Zoom()
{
  setZoomFactor(1.0);
}

Zoom::Zoom(TJBox_Float64 iZoomFactor)
{
  setZoomFactor(iZoomFactor);
}

void Zoom::setZoomFactor(TJBox_Float64 iZoomFactor)
{
  fZoomFactor = toIntZoomFactor(iZoomFactor);
  reset();
}

void Zoom::reset()
{
  fZoomAlgorithm.init(fZoomFactor);
  fLastPercent = 0;
}

void Zoom::reset(TJBox_Float64 iSample, int iPercent)
{
  JBOX_ASSERT(iPercent <= FLOAT_TO_INT_FACTOR);

  fZoomAlgorithm.init(fZoomFactor);
  fZoomAlgorithm.start(iSample, iPercent);
  fLastPercent = fZoomFactor - iPercent;
}

bool Zoom::nextZoomedValue(TJBox_Float64 iSample, CVPoint &oNextZoomedValue)
{
  if(isNoZoom())
  {
    oNextZoomedValue.fAvg = iSample;
    oNextZoomedValue.fMin = iSample;
    oNextZoomedValue.fMax = iSample;

    return true;
  }

  if(fLastPercent == 0)
  {
    fZoomAlgorithm.start(iSample, FLOAT_TO_INT_FACTOR);
    fLastPercent = fZoomFactor - FLOAT_TO_INT_FACTOR;
    return false;
  }

  if(fLastPercent <= FLOAT_TO_INT_FACTOR)
  {
    oNextZoomedValue = fZoomAlgorithm.end(iSample, fLastPercent);
    fLastPercent = FLOAT_TO_INT_FACTOR - fLastPercent;
    if(fLastPercent > 0)
    {
      fZoomAlgorithm.start(iSample, fLastPercent);
      fLastPercent = fZoomFactor - fLastPercent;
    }
    return true;
  }

  fZoomAlgorithm.next(iSample);
  fLastPercent -= FLOAT_TO_INT_FACTOR;
  return false;
}

////////////////////////////////////////////////////////////
// ZoomWindow
////////////////////////////////////////////////////////////

ZoomWindow::ZoomWindow(int iVisibleWindowSize, CircularBuffer<TJBox_Float64> const &iBuffer) :
  fVisibleWindowSize(iVisibleWindowSize),
  fWindowOffset(MAX_WINDOW_OFFSET),
  fBuffer(iBuffer)
{
  JBOX_ASSERT(iVisibleWindowSize > 0);

  fMaxZoomFactor = fBuffer.getSize() / iVisibleWindowSize;

  setZoomFactor(1.0);
}

////////////////////////////////////////////////////////////
// ZoomWindow::setZoomFactor
// Recomputes the zoom points based on the factor
////////////////////////////////////////////////////////////
void ZoomWindow::setZoomFactor(TJBox_Float64 iZoomFactorPercent)
{
  fZoom.setZoomFactor(iZoomFactorPercent * (1 - fMaxZoomFactor) + fMaxZoomFactor);

  if(fZoom.isNoZoom())
  {
    fMinWindowOffset = -fBuffer.getSize() + fVisibleWindowSize - 1;
    fPointsSizeInSamples = MAX_ZOOM_POINTS;
  }
  else
  {
    int percent = FLOAT_TO_INT_FACTOR;
    int offset = 0;

    for(int i = 0; i < MAX_ZOOM_POINTS; i++)
    {
      ZoomPoint &zp = fPoints[i];
      zp.fPercentFirstSample = percent;
      zp.fBufferOffset = offset;

      offset++;

      percent = fZoom.fZoomFactor - percent;

      while(percent >= FLOAT_TO_INT_FACTOR)
      {
        offset++;
        percent -= FLOAT_TO_INT_FACTOR;
      }

      if(percent != 0)
      {
        percent = FLOAT_TO_INT_FACTOR - percent;
      }
      else
      {
        percent = FLOAT_TO_INT_FACTOR;
      }
    }

    fPointsSizeInSamples = offset;
    fMinWindowOffset = - (int) (((float) fBuffer.getSize()) / fPointsSizeInSamples * MAX_ZOOM_POINTS) + fVisibleWindowSize - 1;
  }

//  JBOX_TRACE((std::string("setZoomFactor=")
//    + " fZoomFactor " + std::to_string(fZoom.fZoomFactor)
//    + " fPointsSizeInSamples=" + std::to_string(fPointsSizeInSamples)
//    + " fMinWindowOffset=" + std::to_string(fMinWindowOffset)
//             ).c_str());

  // should never happen...
  JBOX_ASSERT(fMinWindowOffset <= MAX_WINDOW_OFFSET);

  // we make sure that fWindowOffset remains within its bounds!
  fWindowOffset = clamp2(fWindowOffset, fMinWindowOffset, MAX_WINDOW_OFFSET);
}

////////////////////////////////////////////////////////////
// ZoomWindow::setZoomFactor
// Recomputes the zoom points based on the factor and page offset
////////////////////////////////////////////////////////////
int ZoomWindow::setZoomFactor(TJBox_Float64 iZoomFactorPercent, int iInputPageOffset)
{
  // index in the window
  int windowIdx = fWindowOffset - fVisibleWindowSize + 1 + iInputPageOffset;

  // first determine the offset prior to zooming from the window idx
  int bufferOffset;
  CVPoint zoomValue;

  if(fZoom.isNoZoom())
    bufferOffset = windowIdx;
  else
  {
    const ZoomPoint &zoomPoint = getZoomPoint(windowIdx);
    bufferOffset = zoomPoint.fBufferOffset;
    zoomValue = computeZoomValue(zoomPoint);
  }

//  JBOX_TRACE((std::string("setZoomFactor(TJBox_Float64 iZoomFactor, int iInputPageOffset)")
//    + " iInputPageOffset=" + std::to_string(iInputPageOffset)
//    + " windowIdx=" + std::to_string(windowIdx)
//    + " bufferOffset=" + std::to_string(bufferOffset)
//             ).c_str());

  // second apply zoom => this will change fPoints, fMinWindowOffset, etc...
  setZoomFactor(iZoomFactorPercent);

  // when we are all the way on the right no need to compute anything
  if(windowIdx == MAX_WINDOW_OFFSET)
    return MAX_HISTORY_OFFSET;

  // when we are all the way on the left no need to compute anything
  if(windowIdx == fMinWindowOffset)
    return MIN_HISTORY_OFFSET;

  // third determine new window offset
  if(fZoom.isNoZoom())
    windowIdx = bufferOffset;
  else
    windowIdx = findClosestWindowIndex(bufferOffset, zoomValue);

  // we compute the window offset from the window index
  fWindowOffset = windowIdx - iInputPageOffset + fVisibleWindowSize - 1;

  // we make sure that fWindowOffset remains within its bounds!
  fWindowOffset = clamp2(fWindowOffset, fMinWindowOffset, MAX_WINDOW_OFFSET);

  if(fMinWindowOffset == MAX_WINDOW_OFFSET)
    return 0;

  // finally compute new inputHistoryOffset
  int inputHistoryOffset =
    (int) round((fWindowOffset - fMinWindowOffset) * static_cast<float>(MAX_HISTORY_OFFSET) /
                  (MAX_WINDOW_OFFSET - fMinWindowOffset));

  return inputHistoryOffset;
}

////////////////////////////////////////////////////////////
// ZoomWindow::computeZoomValue
////////////////////////////////////////////////////////////
CVPoint ZoomWindow::computeZoomValue(int iInputPageOffset) const
{
  int idx = fWindowOffset - fVisibleWindowSize + 1 + iInputPageOffset;

  if(fZoom.isNoZoom())
    return fBuffer.getAt(idx);

  return computeZoomValue(getZoomPoint(idx));
}

////////////////////////////////////////////////////////////
// ZoomWindow::computeZoomValue
////////////////////////////////////////////////////////////
CVPoint ZoomWindow::computeZoomValue(const ZoomWindow::ZoomPoint &iZoomPoint) const
{
  Zoom zoom(fZoom);

  int offset = iZoomPoint.fBufferOffset;

//  JBOX_TRACE((std::string("computeZoomValue=")
//    + " fZoomFactor " + std::to_string(fZoom.fZoomFactor)
//    + " iInputPageOffset=" + std::to_string(iInputPageOffset)
//    + " idx=" + std::to_string(idx)
//    + " offset=" + std::to_string(offset)
//    + " fPercentFirstSample=" + std::to_string(zp.fPercentFirstSample)
//             ).c_str());

  zoom.reset(fBuffer.getAt(offset++), iZoomPoint.fPercentFirstSample);

  CVPoint res;

  while(!zoom.nextZoomedValue(fBuffer.getAt(offset++), res))
  {
    // nothing to do in the loop itself
  }

  return res;
}

////////////////////////////////////////////////////////////
// ZoomWindow::computeZoomWindow
////////////////////////////////////////////////////////////
bool ZoomWindow::computeZoomWindow(IZoomCallback &callback)
{
  bool res = false;

  if(fZoom.isNoZoom())
  {
    int offset = fWindowOffset - fVisibleWindowSize + 1;

    for(int i = 0; i < fVisibleWindowSize; ++i)
    {
      res |= callback.zoomValue(i, fBuffer.getAt(offset++));
    }
    return res;
  }

  const ZoomPoint zp = getZoomPointLeftOfScreen();

  int offset = zp.fBufferOffset;

  fZoom.reset(fBuffer.getAt(offset++), zp.fPercentFirstSample);

  for(int i = 0; i < fVisibleWindowSize; ++i)
  {
    CVPoint nextZoomedValue;

    while(!fZoom.nextZoomedValue(fBuffer.getAt(offset++), nextZoomedValue))
    {
      // nothing to do in the loop itself
    }

    res |= callback.zoomValue(i, nextZoomedValue);
  }

  return res;
}

////////////////////////////////////////////////////////////
// ZoomWindow::getZoomPoint
////////////////////////////////////////////////////////////
ZoomWindow::ZoomPoint ZoomWindow::getZoomPoint(int iIdx) const
{
  JBOX_ASSERT(iIdx >= minWindowIdx() && iIdx <= MAX_WINDOW_OFFSET);

  ZoomWindow::ZoomPoint res;

  // take into account how many multiples of fPoints we have to iterate
  int offset = -fPointsSizeInSamples;
  offset += iIdx / MAX_ZOOM_POINTS * fPointsSizeInSamples;

  // determine which fPoints to use
  int pointsIdx = iIdx % MAX_ZOOM_POINTS;
  if(pointsIdx == 0)
    offset += fPointsSizeInSamples;
  else
    pointsIdx += MAX_ZOOM_POINTS; // because iIdx is negative, the modulo will be negative as well...

  JBOX_ASSERT(pointsIdx >= 0 && pointsIdx < MAX_ZOOM_POINTS);

  ZoomWindow::ZoomPoint const &point = fPoints[pointsIdx];

  res.fBufferOffset = offset + point.fBufferOffset;
  res.fPercentFirstSample = point.fPercentFirstSample;

//  JBOX_TRACE((std::string("getZoomPoint=")
//    + " iIdx=" + std::to_string(iIdx)
//    + " offset=" + std::to_string(offset)
//    + " res.fOffset=" + std::to_string(res.fBufferOffset)
//    + " fBuffer.getSize=" + std::to_string(fBuffer.getSize())
//             ).c_str());

  // we make sure that we don't roll around the buffer (since it is a circular buffer)
  JBOX_ASSERT(res.fBufferOffset >= -fBuffer.getSize());

  return res;
}

////////////////////////////////////////////////////////////
// ZoomWindow::getZoomPointLeftOfScreen
////////////////////////////////////////////////////////////
ZoomWindow::ZoomPoint ZoomWindow::getZoomPointLeftOfScreen() const
{
  return getZoomPoint(fWindowOffset - fVisibleWindowSize + 1);
}

////////////////////////////////////////////////////////////
// ZoomWindow::findClosestWindowIndex
////////////////////////////////////////////////////////////
int ZoomWindow::findClosestWindowIndex(int iBufferOffset, CVPoint const &iZoomValue) const
{
//  JBOX_TRACE((std::string("fcwi ")
//    + " iBufferOffset=" + std::to_string(iBufferOffset)
//    + " iZoomValue=" + std::to_string(iZoomValue)
//             ).c_str());

  int i1 = minWindowIdx();
  int i2 = MAX_WINDOW_OFFSET;

  int mid = MAX_WINDOW_OFFSET;

  ZoomWindow::ZoomPoint zp;

  while(i1 <= i2)
  {
    mid = i1 + (i2 - i1) / 2;

    zp = getZoomPoint(mid);

//    JBOX_TRACE((std::string("loop =>")
//      + " i1=" + std::to_string(i1)
//      + " i2=" + std::to_string(i2)
//      + " mid=" + std::to_string(mid)
//      + " zp.fBufferOffset=" + std::to_string(zp.fBufferOffset)
//               ).c_str());

    if(zp.fBufferOffset == iBufferOffset)
      break;

    if(zp.fBufferOffset < iBufferOffset)
      i1 = mid + 1;
    else
      i2 = mid -1;
  }

  TJBox_Float64 minZoomDelta = fabs(iZoomValue.fAvg - computeZoomValue(zp).fAvg);
  int minBufferOffsetDelta = abs(iBufferOffset - zp.fBufferOffset);

  // we found the exact point... no need to go further...
  if(minBufferOffsetDelta == 0 && equals5DP(minZoomDelta, 0.0))
    return mid;

  int res = mid;

//  JBOX_TRACE((std::string("mid =>")
//    + " zp.fBufferOffset=" + std::to_string(zp.fBufferOffset)
//    + "/" + std::to_string(minBufferOffsetDelta)
//    + " zoomValue="
//    + "/" + std::to_string(minZoomDelta)
//             ).c_str());

  // determine the best point
  for(int i = -2; i <= 2; i++)
  {
    int idx = clamp2(mid + i, minWindowIdx(), MAX_WINDOW_OFFSET);

    // skipping mid (already done)
    if(idx == mid)
      continue;

    const ZoomPoint &point = getZoomPoint(idx);
    TJBox_Float64 delta = fabs(iZoomValue.fAvg - computeZoomValue(point).fAvg);

    if(delta < minZoomDelta)
    {
      res = idx;
      minZoomDelta = delta;
      minBufferOffsetDelta = abs(iBufferOffset - point.fBufferOffset);
    }

    if(delta == minZoomDelta)
    {
      int bufferOffsetDelta = abs(iBufferOffset - point.fBufferOffset);
      if(bufferOffsetDelta < minBufferOffsetDelta)
      {
        res = idx;
        minZoomDelta = delta;
        minBufferOffsetDelta = bufferOffsetDelta;
      }
    }

//    JBOX_TRACE((std::string("trying =>")
//      + " i=" + std::to_string(i)
//      + " zp.fBufferOffset=" + std::to_string(point.fBufferOffset)
//      + "/" + std::to_string(abs(iBufferOffset - point.fBufferOffset))
//      + " zoomValue="
//      + "/" + std::to_string(delta)
//               ).c_str());
  }

//  JBOX_TRACE((std::string("closest => ") + std::to_string(res)
//             ).c_str());

  // did not find an exact match but las one should be "close" enough
  return res;
}

////////////////////////////////////////////////////////////
// ZoomWindow::setWindowOffset
////////////////////////////////////////////////////////////
void ZoomWindow::setWindowOffset(int iInputHistoryOffset)
{
  switch(iInputHistoryOffset)
  {
    case MIN_HISTORY_OFFSET:
      fWindowOffset = fMinWindowOffset;
      break;

    case MAX_HISTORY_OFFSET:
      fWindowOffset = MAX_WINDOW_OFFSET;
      break;

    default:
      fWindowOffset = (int)
        (static_cast<float>(MAX_WINDOW_OFFSET - fMinWindowOffset) / MAX_HISTORY_OFFSET * iInputHistoryOffset + fMinWindowOffset);
      break;
  }

//  JBOX_TRACE((std::string("setWindowOffset=")
//    + " iInputHistoryOffset=" + std::to_string(iInputHistoryOffset)
//    + " fWindowOffset=" + std::to_string(fWindowOffset)
//             ).c_str());

  // sanity check
  JBOX_ASSERT(fWindowOffset >= fMinWindowOffset && fWindowOffset <= MAX_WINDOW_OFFSET);
}
