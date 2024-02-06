#pragma once

#include "window.h"
#include "char_dimensions.h"

namespace applications
{
  class WhatSizeApp : public shared::Window
  {
    friend class Window;

  private:
    WhatSizeApp();

    LRESULT HandleCreate(shared::MessageProcParameters mpp);
    LRESULT HandlePaint(shared::MessageProcParameters mpp);
    LRESULT HandleDestroy(shared::MessageProcParameters mpp);

    void PrintMappingModeDimensions(HDC dc, int x, int y, int map_mode, const std::wstring& map_mode_name);

    sys_info_types::CharDimensions char_dimensions_;
  };
}