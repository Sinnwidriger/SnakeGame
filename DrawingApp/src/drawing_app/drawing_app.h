#pragma once

#include "shared_components/gdi_obj.h"
#include "shared_components/window.h"

enum class DrawAppContent
{
  kDrawNothing = 0,
  kDrawSinWave = 1,
  kDrawLines = 2,
  kDrawBezier = 3,
  kDrawPolygon = 4,
  kDrawClover = 5
};

class DrawingApp;

using ResizeCallback = std::function<void()>;
using ResizeMethodPtr = void(DrawingApp::*)();

using DrawCallback = std::function<void(HDC, PAINTSTRUCT)>;
using DrawMethodPtr = void(DrawingApp::*)(HDC, PAINTSTRUCT);

class DrawingApp : public shared::Window
{
  friend class Window;

public:
  void SetContentType(DrawAppContent content_type);

private:
  DrawingApp();

  virtual void Idle() override;

  static DrawCallback DrawMethodToCallback(DrawingApp* app_instance, DrawMethodPtr method_ptr);
  static ResizeCallback ResizeMethodToCallback(DrawingApp* app_instance, ResizeMethodPtr method_ptr);

#pragma region message_handlers
  LRESULT HandleSize(shared::MessageProcParameters mpp);
  LRESULT HandleMouse(shared::MessageProcParameters mpp);
  LRESULT HandlePaint(shared::MessageProcParameters mpp);
  LRESULT HandleDestroy(shared::MessageProcParameters mpp);
#pragma endregion

#pragma region draw_resize_methods
  void InitializeNothing();
  void DrawNothing(HDC dc, PAINTSTRUCT ps);

  void InitializeSinWavePoints();
  void DrawSinWave(HDC dc, PAINTSTRUCT ps);

  void InitializeLinesPoints();
  void DrawLines(HDC dc, PAINTSTRUCT ps);
  
  void InitializeBezierPoints();
  void DrawBezier(HDC dc, PAINTSTRUCT ps);
  
  void InitializePolygonPoints();
  void DrawPolygon(HDC dc, PAINTSTRUCT ps);

  void InitializeCloverRegion();
  void DrawClover(HDC dc, PAINTSTRUCT ps);
#pragma endregion

#pragma region fields
  DrawAppContent draw_content_;
  std::unordered_map<DrawAppContent, ResizeCallback> resize_callback_map_;
  std::unordered_map<DrawAppContent, DrawCallback> draw_callback_map_;

  std::vector<POINT> sin_wave_points_;
  std::vector<POINT> lines_points_;
  std::vector<POINT> bezier_points_;
  std::vector<POINT> polygon_points_;
  shared::GDIObj<HRGN> clover_region_;

  int client_area_width_;
  int client_area_height_;
#pragma endregion
};