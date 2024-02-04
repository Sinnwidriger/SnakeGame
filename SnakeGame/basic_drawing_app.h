#pragma once

#include "window.h"

namespace applications
{

  enum class DrawAppContentType
  {
    kDrawSinWave = 0,
    kDrawLines = 1
  };

  class BasicDrawingApp : public shared::Window
  {
    friend class Window;

  public:
    void SetContentType(DrawAppContentType content_type);

  private:
    BasicDrawingApp();

    LRESULT HandleSize(shared::MessageProcParameters mpp);
    LRESULT HandlePaint(shared::MessageProcParameters mpp);
    LRESULT HandleDestroy(shared::MessageProcParameters mpp);

    virtual LRESULT HandleMessage(shared::MessageProcParameters mpp) override;

    void InitializeSinWavePoints(int client_area_width, int client_area_height);
    void DrawSinWave(HDC dc, PAINTSTRUCT ps);
    void DrawLines(HDC dc, PAINTSTRUCT ps);

    // Fields
    std::function<void(HDC, PAINTSTRUCT)> draw_function_callback_;
    std::vector<POINT> sin_wave_points_;
  };

}