#include "pch.h"
#include "what_size_app.h"

namespace applications
{
  WhatSizeApp::WhatSizeApp() :
    shared::Window(L"What Size App"),
    char_dimensions_(sys_info_types::CharDimensions())
  {
    AddMessageCallback(WM_CREATE, static_cast<shared::MessageCallbackFunction>(&WhatSizeApp::HandleCreate));
    AddMessageCallback(WM_PAINT, static_cast<shared::MessageCallbackFunction>(&WhatSizeApp::HandlePaint));
    AddMessageCallback(WM_DESTROY, static_cast<shared::MessageCallbackFunction>(&WhatSizeApp::HandleDestroy));
  }

  LRESULT WhatSizeApp::HandleCreate(shared::MessageProcParameters mpp)
  {
    TEXTMETRIC tm;
    HDC dc = GetDC(wnd_);
    SelectObject(dc, GetStockObject(SYSTEM_FIXED_FONT));
    
    GetTextMetrics(dc, &tm);
    char_dimensions_.lower_case_width = tm.tmAveCharWidth;
    char_dimensions_.height = tm.tmHeight + tm.tmExternalLeading;

    ReleaseDC(wnd_, dc);

    return 0;
  }

  LRESULT WhatSizeApp::HandlePaint(shared::MessageProcParameters mpp)
  {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(wnd_, &ps);
    SelectObject(dc, GetStockObject(SYSTEM_FIXED_FONT));

    SetMapMode(dc, MM_ANISOTROPIC);
    SetWindowExtEx(dc, 1, 1, nullptr);
    SetViewportExtEx(dc, char_dimensions_.lower_case_width, char_dimensions_.height, nullptr);

    std::wstringstream heading;
    heading
      << std::left << std::setw(25) << L"Mapping Mode"
      << std::right << std::setw(10) << L"Left"
      << std::setw(10) << L"Right"
      << std::setw(10) << L"Top"
      << std::setw(10) << L"Bottom";

    std::wstringstream underline;
    underline
      << std::left << std::setw(25) << L"------------"
      << std::right << std::setw(10) << L"----"
      << std::setw(10) << L"-----"
      << std::setw(10) << L"---"
      << std::setw(10) << L"------";

    TextOut(dc, 1, 1, heading.str().c_str(), heading.str().size());
    TextOut(dc, 1, 2, underline.str().c_str(), underline.str().size());

    PrintMappingModeDimensions(dc, 1, 3, MM_TEXT, L"TEXT (pixels)");
    PrintMappingModeDimensions(dc, 1, 4, MM_LOMETRIC, L"LOMETRIC (.1 mm)");
    PrintMappingModeDimensions(dc, 1, 5, MM_HIMETRIC, L"MM_HIMETRIC (.01 mm)");
    PrintMappingModeDimensions(dc, 1, 6, MM_LOENGLISH, L"MM_LOENGLISH (.01 in)");
    PrintMappingModeDimensions(dc, 1, 7, MM_HIENGLISH, L"MM_HIENGLISH (.001 in)");
    PrintMappingModeDimensions(dc, 1, 8, MM_TWIPS, L"MM_TWIPS (1/1440 in)");

    EndPaint(wnd_, &ps);
    return 0;
  }

  LRESULT WhatSizeApp::HandleDestroy(shared::MessageProcParameters mpp)
  {
    PostQuitMessage(0);
    return 0;
  }

  void WhatSizeApp::PrintMappingModeDimensions(HDC dc, int x, int y, int map_mode, const std::wstring& map_mode_name)
  {
    RECT client_area;
    SaveDC(dc);

    SetMapMode(dc, map_mode); 
    GetClientRect(wnd_, &client_area);
    DPtoLP(dc, reinterpret_cast<PPOINT>(&client_area), 2);

    RestoreDC(dc, -1);

    std::wstringstream mapping_mode_dimension_line;
    mapping_mode_dimension_line
      << std::left << std::setw(25) << map_mode_name
      << std::right << std::setw(10) << client_area.left
      << std::setw(10) << client_area.right
      << std::setw(10) << client_area.top
      << std::setw(10) << client_area.bottom;

    TextOut(dc, x, y, mapping_mode_dimension_line.str().c_str(), mapping_mode_dimension_line.str().size());
  }
}