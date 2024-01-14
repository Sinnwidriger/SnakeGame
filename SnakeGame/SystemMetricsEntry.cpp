#include "pch.h"
#include "SystemMetricsEntry.h"

SystemMetricsEntry::SystemMetricsEntry(int index, std::wstring name, std::wstring description) :
    m_iIndex(index), m_szName(name), m_szDescription(description), m_iNumericValue(GetSystemMetrics(index))
{ }