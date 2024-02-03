#include "pch.h"
#include "device_capability.h"

namespace shared
{

DeviceCapability::DeviceCapability(int index, std::wstring name, std::wstring description) :
  SystemInformation(index, name, description)
{ }

void DeviceCapability::InitializeNumericValue(HDC dc)
{
  numeric_value_ = std::to_wstring(GetDeviceCaps(dc, index_));
}

}