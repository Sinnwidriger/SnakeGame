#pragma once

enum class DrawAppContentType
{
  kDrawNothing = 0,
  kDrawSinWave = 1,
  kDrawLines = 2,
  kDrawBezier = 3,
  kDrawPolygon = 4,
  kDrawClover = 5
};

class DrawingApp;

using DrawFunctionCallback = void(DrawingApp::*)(HDC, PAINTSTRUCT);

class DrawingApp : public shared::Window
{
  friend class Window;

public:
  void SetContentType(DrawAppContentType content_type);

private:
  DrawingApp();

  LRESULT HandleSize(shared::MessageProcParameters mpp);
  LRESULT HandleMouse(shared::MessageProcParameters mpp);
  LRESULT HandlePaint(shared::MessageProcParameters mpp);
  LRESULT HandleDestroy(shared::MessageProcParameters mpp);

  virtual void Idle() override;

  auto GetBindedDrawFunctionCallback(DrawFunctionCallback callback) -> std::function<void(HDC, PAINTSTRUCT)>;

  void InitializeSinWavePoints();
  void InitializeBezierPoints();
  void InitializePolygonPoints();
  void InitializeCloverRegion();

  void DrawNothing(HDC dc, PAINTSTRUCT ps);
  void DrawSinWave(HDC dc, PAINTSTRUCT ps);
  void DrawLines(HDC dc, PAINTSTRUCT ps);
  void DrawBezier(HDC dc, PAINTSTRUCT ps);
  void DrawPolygon(HDC dc, PAINTSTRUCT ps);
  void DrawClover(HDC dc, PAINTSTRUCT ps);

  // Fields
  DrawAppContentType draw_content_type_;
  std::unordered_map<DrawAppContentType, std::function<void(HDC, PAINTSTRUCT)>> draw_function_callback_map_;
  std::vector<POINT> sin_wave_points_;
  std::vector<POINT> bezier_points_;
  std::vector<POINT> polygon_points_;

  shared::GDIObj<HRGN> clover_region_ = CreateRectRgn(0, 0, 1, 1);

  int client_area_width_;
  int client_area_height_;
};