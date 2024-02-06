#pragma once

enum class DrawAppContentType
  {
    kDrawSinWave = 0,
    kDrawLines = 1,
    kDrawBezier = 2,
    kDrawPolygon = 3
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

    auto GetBindedDrawFunctionCallback(DrawFunctionCallback callback) -> std::function<void(HDC, PAINTSTRUCT)>;

    void InitializeSinWavePoints(int client_area_width, int client_area_height);
    void InitializeBezierPoints(int client_area_width, int client_area_height);
    void InitializePolygonPoints(int client_area_width, int client_area_height);

    void DrawSinWave(HDC dc, PAINTSTRUCT ps);
    void DrawLines(HDC dc, PAINTSTRUCT ps);
    void DrawBezier(HDC dc, PAINTSTRUCT ps);
    void DrawPolygon(HDC dc, PAINTSTRUCT ps);

    // Fields
    DrawAppContentType draw_content_type_;
    std::unordered_map<DrawAppContentType, std::function<void(HDC, PAINTSTRUCT)>> draw_function_callback_map_;
    std::vector<POINT> sin_wave_points_;
    std::vector<POINT> bezier_points_;
    std::vector<POINT> polygon_points_;
  };