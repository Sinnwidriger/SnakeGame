#pragma once

#include "pch.h"

class SystemMetricsEntry
{
public:
	SystemMetricsEntry(int index, std::wstring name, std::wstring description);

public:
	inline int GetIndex() const { return m_iIndex; }
	inline std::wstring GetName() const { return m_szName; }
	inline std::wstring GetDescription() const { return m_szDescription; }
	inline int GetNumericValue() const { return m_iNumericValue; }

private:
	int m_iIndex;
	std::wstring m_szName;
	std::wstring m_szDescription;
	int m_iNumericValue;
};

