#include "pch.h"
#include "system_information.h"

namespace shared
{

SystemInformation::SystemInformation(int index, std::wstring name, std::wstring description) :
  index_(index), name_(name), description_(description)
{ }

}


