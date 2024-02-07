#pragma once

struct CharDimensions
{
  unsigned int lower_case_width;
  unsigned int upper_case_width;
  unsigned int height;
};

class WhatSizeApp : public shared::Window
{
  friend class Window;

private:
  WhatSizeApp();

  LRESULT HandleCreate(shared::MessageProcParameters mpp);
  LRESULT HandlePaint(shared::MessageProcParameters mpp);
  LRESULT HandleDestroy(shared::MessageProcParameters mpp);

  void PrintMappingModeDimensions(HDC dc, int x, int y, int map_mode, const std::wstring& map_mode_name);

  CharDimensions char_dimensions_;
};