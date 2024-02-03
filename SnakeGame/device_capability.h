#ifndef SNAKE_GAME_SHARED_DEVICE_CAPABILITY_H_
#define SNAKE_GAME_SHARED_DEVICE_CAPABILITY_H_

#include "system_information.h"

namespace shared
{

class DeviceCapability : public SystemInformation
{
public:
	DeviceCapability() = default;
	DeviceCapability(int index, std::wstring name, std::wstring description);

	void InitializeNumericValue(HDC dc);
};

}
#endif