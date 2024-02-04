#pragma once

#include "system_information.h"

namespace sys_info_types
{

	class DeviceCapability : public SystemInformation
	{
	public:
		DeviceCapability() = default;
		DeviceCapability(int index, std::wstring name, std::wstring description);

		void InitializeNumericValue(HDC dc);
	};

}