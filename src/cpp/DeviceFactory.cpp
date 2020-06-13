#include "JukeboxExports.h"
#include "Device.h"
#include <logging/logging.h>

CommonDevice *createDevice(const TJBox_Value iParams[], TJBox_UInt32 iCount)
{
  DCHECK_F(iCount >= 1);

  TJBox_Float64 sampleRate = JBox_GetNumber(iParams[0]);
  int sampleRateInt = static_cast<int>(sampleRate);

  DLOG_F(INFO, "New instance with sample rate => %d", sampleRateInt);

  return new Device(sampleRateInt);
}
