#pragma once

#ifdef SHAREDCOMPONENTS_EXPORTS
#define SHAREDCOMPONENTS_API __declspec(dllexport)
#else
#define SHAREDCOMPONENTS_API __declspec(dllimport)
#endif

class SHAREDCOMPONENTS_API Test
{
public:
  static int Foo();
};