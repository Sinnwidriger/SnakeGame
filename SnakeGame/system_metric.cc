#include "pch.h"
#include "system_metric.h"

namespace shared
{

SystemMetric::SystemMetric(int index, std::wstring name, std::wstring description) :
  SystemInformation(index, name, description)
{ }

void SystemMetric::InitializeNumericValue()
{
  numeric_value_ = std::to_wstring(GetSystemMetrics(index_));
}

}