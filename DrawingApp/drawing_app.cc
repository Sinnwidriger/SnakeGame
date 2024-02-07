#include "stdafx.h"
#include "drawing_app.h"

void DrawingApp::SetContentType(DrawAppContentType content_type)
{
	draw_content_type_ = content_type;
	InvalidateRect(wnd_, nullptr, TRUE);
}

DrawingApp::DrawingApp() :
	shared::Window(L"Basic Drawing Application")
{
	AddMessageCallback(WM_SIZE, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandleSize));
	AddMessageCallback(WM_PAINT, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandlePaint));
	AddMessageCallback(WM_DESTROY, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandleDestroy));

	AddMessageCallback(WM_MOUSEMOVE, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandleMouse));
	AddMessageCallback(WM_LBUTTONDOWN, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandleMouse));
	AddMessageCallback(WM_RBUTTONDOWN, static_cast<shared::MessageCallbackFunction>(&DrawingApp::HandleMouse));

	draw_function_callback_map_[DrawAppContentType::kDrawNothing] = GetBindedDrawFunctionCallback(&DrawingApp::DrawNothing);
	draw_function_callback_map_[DrawAppContentType::kDrawSinWave] = GetBindedDrawFunctionCallback(&DrawingApp::DrawSinWave);
	draw_function_callback_map_[DrawAppContentType::kDrawLines] = GetBindedDrawFunctionCallback(&DrawingApp::DrawLines);
	draw_function_callback_map_[DrawAppContentType::kDrawBezier] = GetBindedDrawFunctionCallback(&DrawingApp::DrawBezier);
	draw_function_callback_map_[DrawAppContentType::kDrawPolygon] = GetBindedDrawFunctionCallback(&DrawingApp::DrawPolygon);
	draw_function_callback_map_[DrawAppContentType::kDrawClover] = GetBindedDrawFunctionCallback(&DrawingApp::DrawClover);
}

LRESULT DrawingApp::HandleSize(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	client_area_width_ = LOWORD(lparam);
	client_area_height_ = HIWORD(lparam);

	InitializeSinWavePoints();
	InitializeBezierPoints();
	InitializePolygonPoints();
	InitializeCloverRegion();

	return 0;
}

LRESULT DrawingApp::HandleMouse(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	if (wparam & MK_LBUTTON || wparam & MK_RBUTTON)
	{
		bezier_points_[wparam] = {
			.x = LOWORD(lparam),
			.y = HIWORD(lparam)
		};
		InvalidateRect(wnd_, nullptr, TRUE);
	}

	return 0;
}

LRESULT DrawingApp::HandlePaint(shared::MessageProcParameters mpp)
{
	PAINTSTRUCT ps;

	HDC dc = BeginPaint(wnd_, &ps);
	SaveDC(dc);

	draw_function_callback_map_[draw_content_type_](dc, ps);

	RestoreDC(dc, -1);
	EndPaint(wnd_, &ps);

	return 0;
}

LRESULT DrawingApp::HandleDestroy(shared::MessageProcParameters mpp)
{
	PostQuitMessage(0);
	return 0;
}

void DrawingApp::Idle()
{
	if (draw_content_type_ == DrawAppContentType::kDrawNothing)
	{
		HDC dc = GetDC(wnd_);
		RECT rect;

		if (client_area_width_ == 0 || client_area_height_ == 0)
			return;

		shared::GDIObj<HBRUSH> brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));

		SetRect(&rect, rand() % client_area_width_, rand() % client_area_height_, rand() % client_area_width_, rand() % client_area_height_);
		FillRect(dc, &rect, brush);

		ReleaseDC(wnd_, dc);

		Sleep(100);
	}
}

auto DrawingApp::GetBindedDrawFunctionCallback(DrawFunctionCallback callback) -> std::function<void(HDC, PAINTSTRUCT)>
{
	return std::bind(callback, this, std::placeholders::_1, std::placeholders::_2);
}

void DrawingApp::InitializeSinWavePoints()
{
	sin_wave_points_.clear();
	for (int i = 0; i < 1000; ++i)
	{
		POINT pt = {
			.x = i * client_area_width_ / 1000,
			.y = static_cast<int>(client_area_height_ / 2 * (1 - std::sin(2 * std::numbers::pi * i / 1000)))
		};

		sin_wave_points_.push_back(pt);
	}
}

void DrawingApp::InitializeBezierPoints()
{
	bezier_points_.clear();

	bezier_points_.push_back({ client_area_width_ / 4, client_area_height_ / 2 });
	bezier_points_.push_back({ client_area_width_ / 2, client_area_height_ / 4 });
	bezier_points_.push_back({ client_area_width_ / 2, client_area_height_ * 3 / 4 });
	bezier_points_.push_back({ client_area_width_ * 3 / 4, client_area_height_ / 2 });
}

void DrawingApp::InitializePolygonPoints()
{
	polygon_points_.clear();

	polygon_points_.push_back({ 10 * client_area_width_ / 200, 70 * client_area_height_ / 100 });
	polygon_points_.push_back({ 50 * client_area_width_ / 200, 70 * client_area_height_ / 100 });
	polygon_points_.push_back({ 50 * client_area_width_ / 200, 10 * client_area_height_ / 100 });
	polygon_points_.push_back({ 90 * client_area_width_ / 200, 10 * client_area_height_ / 100 });
	polygon_points_.push_back({ 90 * client_area_width_ / 200, 50 * client_area_height_ / 100 });
	polygon_points_.push_back({ 30 * client_area_width_ / 200, 50 * client_area_height_ / 100 });
	polygon_points_.push_back({ 30 * client_area_width_ / 200, 90 * client_area_height_ / 100 });
	polygon_points_.push_back({ 70 * client_area_width_ / 200, 90 * client_area_height_ / 100 });
	polygon_points_.push_back({ 70 * client_area_width_ / 200, 30 * client_area_height_ / 100 });
	polygon_points_.push_back({ 10 * client_area_width_ / 200, 30 * client_area_height_ / 100 });
}

void DrawingApp::InitializeCloverRegion()
{
	shared::GDIObj<HRGN> left_leaf = CreateEllipticRgn(0, client_area_height_ / 3, client_area_width_ / 2, client_area_height_ * 2 / 3);
	shared::GDIObj<HRGN> right_leaf = CreateEllipticRgn(client_area_width_ / 2, client_area_height_ / 3, client_area_width_, client_area_height_ * 2 / 3);
	shared::GDIObj<HRGN> top_leaf = CreateEllipticRgn(client_area_width_ / 3, 0, client_area_width_ * 2 / 3, client_area_height_ / 2);
	shared::GDIObj<HRGN> bottom_leaf = CreateEllipticRgn(client_area_width_ / 3, client_area_height_ / 2, client_area_width_ * 2 / 3, client_area_height_);
	
	shared::GDIObj<HRGN> horz_leaves = CreateRectRgn(0, 0, 1, 1);
	shared::GDIObj<HRGN> vert_leaves = CreateRectRgn(0, 0, 1, 1);

	CombineRgn(horz_leaves, left_leaf, right_leaf, RGN_OR);
	CombineRgn(vert_leaves, top_leaf, bottom_leaf, RGN_OR);
	CombineRgn(clover_region_, horz_leaves, vert_leaves, RGN_XOR);
}

void DrawingApp::DrawNothing(HDC dc, PAINTSTRUCT ps)
{ }

void DrawingApp::DrawSinWave(HDC dc, PAINTSTRUCT ps)
{
	MoveToEx(dc, 0, ps.rcPaint.bottom / 2, nullptr);
	LineTo(dc, ps.rcPaint.right, ps.rcPaint.bottom / 2);
	Polyline(dc, sin_wave_points_.data(), static_cast<int>(sin_wave_points_.size()));
}

void DrawingApp::DrawLines(HDC dc, PAINTSTRUCT ps)
{
	int client_width = ps.rcPaint.right;
	int client_height = ps.rcPaint.bottom;

	Rectangle(dc, client_width / 8, client_height / 8, client_width * 7 / 8, client_height * 7 / 8);

	MoveToEx(dc, 0, 0, nullptr);
	LineTo(dc, client_width, client_height);

	MoveToEx(dc, 0, client_height, nullptr);
	LineTo(dc, client_width, 0);

	Ellipse(dc, client_width / 8, client_height / 8, client_width * 7 / 8, client_height * 7 / 8);
	RoundRect(dc, client_width / 4, client_height / 4, client_width * 3 / 4, client_height * 3 / 4, client_width / 4, client_height / 4);
}

void DrawingApp::DrawBezier(HDC dc, PAINTSTRUCT ps)
{
	PolyBezier(dc, bezier_points_.data(), bezier_points_.size());

	MoveToEx(dc, bezier_points_[0].x, bezier_points_[0].y, nullptr);
	LineTo(dc, bezier_points_[1].x, bezier_points_[1].y);


	MoveToEx(dc, bezier_points_[2].x, bezier_points_[2].y, nullptr);
	LineTo(dc, bezier_points_[3].x, bezier_points_[3].y);
}

void DrawingApp::DrawPolygon(HDC dc, PAINTSTRUCT ps)
{
	SelectObject(dc, GetStockObject(GRAY_BRUSH));
	SetPolyFillMode(dc, ALTERNATE);
	Polygon(dc, polygon_points_.data(), polygon_points_.size());

	for (int i = 0; i < polygon_points_.size(); ++i)
	{
		polygon_points_[i].x += ps.rcPaint.right / 2;
	}

	SetPolyFillMode(dc, WINDING);
	Polygon(dc, polygon_points_.data(), polygon_points_.size());
}

void DrawingApp::DrawClover(HDC dc, PAINTSTRUCT ps)
{
	SelectClipRgn(dc, clover_region_);
	SetWindowOrgEx(dc, -client_area_width_ / 2, -client_area_height_ / 2, nullptr);

	double radius = max(client_area_width_ / 2, client_area_height_ / 2);

	for (double angle = 0.0; angle < std::numbers::pi * 2; angle += std::numbers::pi / 180)
	{
		MoveToEx(dc, 0, 0, nullptr);
		LineTo(dc, (int)(radius * std::cos(angle)), (int)(radius * std::sin(angle)));
	}
}
