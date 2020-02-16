#include "JukeboxExports.h"
#include "Device.h"

CommonDevice *createDevice(const TJBox_Value iParams[], TJBox_UInt32 iCount)
{
  JBOX_ASSERT(iCount >= 1);
  TJBox_Float64 sampleRate = JBox_GetNumber(iParams[0]);
  int sampleRateInt = static_cast<int>(sampleRate);
#ifndef  __phdsp__
  JBOX_TRACE((std::string("New instance with sample rate => ") + std::to_string(sampleRate)).c_str());
#endif // !__phdsp__
  return new Device(sampleRateInt);
}
