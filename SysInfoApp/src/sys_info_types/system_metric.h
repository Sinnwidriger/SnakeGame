#pragma once

#include "system_information.h"

namespace sys_info_types
{

	class SystemMetric : public SystemInformation
	{
	public:
		SystemMetric() = default;
		SystemMetric(int index, std::wstring name, std::wstring description);

		void InitializeNumericValue();
	};

}