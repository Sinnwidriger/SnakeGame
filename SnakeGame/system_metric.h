#ifndef SNAKE_GAME_SHARED_SYSTEM_METRIC_H_
#define SNAKE_GAME_SHARED_SYSTEM_METRIC_H_

#include "system_information.h"

namespace shared
{

class SystemMetric : public SystemInformation
{
public:
	SystemMetric() = default;
	SystemMetric(int index, std::wstring name, std::wstring description);

	void InitializeNumericValue();
};

}

#endif