#include "stdafx.h"
#include "drawing_app.h"

void DrawingApp::SetContentType(DrawAppContent content_type)
{
	draw_content_type_ = content_type;
	resize_callback_map_[content_type]();
	InvalidateRect(wnd_, nullptr, TRUE);
}

DrawingApp::DrawingApp() :
	shared::Window(L"Basic Drawing Application"),
	clover_region_(CreateRectRgn(0, 0, 1, 1))
{
	AddMessageCallback(WM_SIZE, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandleSize));
	AddMessageCallback(WM_PAINT, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandlePaint));
	AddMessageCallback(WM_DESTROY, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandleDestroy));

	AddMessageCallback(WM_MOUSEMOVE, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandleMouse));
	AddMessageCallback(WM_LBUTTONDOWN, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandleMouse));
	AddMessageCallback(WM_RBUTTONDOWN, static_cast<shared::MessageMethodPtr>(&DrawingApp::HandleMouse));

	resize_callback_map_[DrawAppContent::kDrawNothing] = ResizeMethodToCallback(this, &DrawingApp::InitializeNothing);
	draw_callback_map_[DrawAppContent::kDrawNothing] = DrawMethodToCallback(this, &DrawingApp::DrawNothing);

	resize_callback_map_[DrawAppContent::kDrawSinWave] = ResizeMethodToCallback(this, &DrawingApp::InitializeSinWavePoints);
	draw_callback_map_[DrawAppContent::kDrawSinWave] = DrawMethodToCallback(this, &DrawingApp::DrawSinWave);

	resize_callback_map_[DrawAppContent::kDrawLines] = ResizeMethodToCallback(this, &DrawingApp::InitializeLinesPoints);
	draw_callback_map_[DrawAppContent::kDrawLines] = DrawMethodToCallback(this, &DrawingApp::DrawLines);

	resize_callback_map_[DrawAppContent::kDrawBezier] = ResizeMethodToCallback(this, &DrawingApp::InitializeBezierPoints);
	draw_callback_map_[DrawAppContent::kDrawBezier] = DrawMethodToCallback(this, &DrawingApp::DrawBezier);

	resize_callback_map_[DrawAppContent::kDrawPolygon] = ResizeMethodToCallback(this, &DrawingApp::InitializePolygonPoints);
	draw_callback_map_[DrawAppContent::kDrawPolygon] = DrawMethodToCallback(this, &DrawingApp::DrawPolygon);

	resize_callback_map_[DrawAppContent::kDrawClover] = ResizeMethodToCallback(this, &DrawingApp::InitializeCloverRegion);
	draw_callback_map_[DrawAppContent::kDrawClover] = DrawMethodToCallback(this, &DrawingApp::DrawClover);
}

#pragma region message_handlers
LRESULT DrawingApp::HandleSize(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	client_area_width_ = LOWORD(lparam);
	client_area_height_ = HIWORD(lparam);

	resize_callback_map_[draw_content_type_]();

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

	draw_callback_map_[draw_content_type_](dc, ps);

	RestoreDC(dc, -1);
	EndPaint(wnd_, &ps);

	return 0;
}

LRESULT DrawingApp::HandleDestroy(shared::MessageProcParameters mpp)
{
	PostQuitMessage(0);
	return 0;
}
#pragma endregion message_handlers

void DrawingApp::Idle()
{
	if (draw_content_type_ == DrawAppContent::kDrawNothing)
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

DrawCallback DrawingApp::DrawMethodToCallback(DrawingApp* app_instance, DrawMethodPtr method_ptr)
{
	return std::bind(method_ptr, app_instance, std::placeholders::_1, std::placeholders::_2);
}

ResizeCallback DrawingApp::ResizeMethodToCallback(DrawingApp* app_instance, ResizeMethodPtr method_ptr)
{
	return std::bind(method_ptr, app_instance);
}

#pragma region draw_resize_methods
void DrawingApp::InitializeNothing()
{ }
void DrawingApp::DrawNothing(HDC dc, PAINTSTRUCT ps)
{ }

#pragma region sin_wave
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

void DrawingApp::DrawSinWave(HDC dc, PAINTSTRUCT ps)
{
	MoveToEx(dc, 0, ps.rcPaint.bottom / 2, nullptr);
	LineTo(dc, ps.rcPaint.right, ps.rcPaint.bottom / 2);
	Polyline(dc, sin_wave_points_.data(), static_cast<int>(sin_wave_points_.size()));
}
#pragma endregion sin_wave

#pragma region lines
void DrawingApp::InitializeLinesPoints()
{
	lines_points_.clear();

	lines_points_.push_back({ client_area_width_ / 8, client_area_height_ / 8 });
	lines_points_.push_back({ client_area_width_ * 7 / 8, client_area_height_ * 7 / 8 });

	lines_points_.push_back({ 0, 0 });
	lines_points_.push_back({ client_area_width_, client_area_height_});

	lines_points_.push_back({ 0, client_area_height_ });
	lines_points_.push_back({ client_area_width_, 0 });

	lines_points_.push_back({ client_area_width_ / 4, client_area_height_ / 4 });
	lines_points_.push_back({ client_area_width_ * 3 / 4, client_area_height_ * 3 / 4 });
}

void DrawingApp::DrawLines(HDC dc, PAINTSTRUCT ps)
{
	Rectangle(dc,
		lines_points_[0].x,
		lines_points_[0].y,
		lines_points_[1].x,
		lines_points_[1].y);

	MoveToEx(dc,
		lines_points_[2].x,
		lines_points_[2].y,
		nullptr);
	LineTo(dc,
		lines_points_[3].x,
		lines_points_[3].y);

	MoveToEx(dc,
		lines_points_[4].x,
		lines_points_[4].y,
		nullptr);
	LineTo(dc,
		lines_points_[5].x,
		lines_points_[5].y);

	Ellipse(dc,
		lines_points_[0].x,
		lines_points_[0].y,
		lines_points_[1].x,
		lines_points_[1].y);

	RoundRect(dc,
		lines_points_[6].x,
		lines_points_[6].y,
		lines_points_[7].x,
		lines_points_[7].y,
		client_area_width_ / 4,
		client_area_height_ / 4);
}
#pragma endregion lines

#pragma region bezier
void DrawingApp::InitializeBezierPoints()
{
	bezier_points_.clear();

	bezier_points_.push_back({ client_area_width_ / 4, client_area_height_ / 2 });
	bezier_points_.push_back({ client_area_width_ / 2, client_area_height_ / 4 });
	bezier_points_.push_back({ client_area_width_ / 2, client_area_height_ * 3 / 4 });
	bezier_points_.push_back({ client_area_width_ * 3 / 4, client_area_height_ / 2 });
}

void DrawingApp::DrawBezier(HDC dc, PAINTSTRUCT ps)
{
	PolyBezier(dc, bezier_points_.data(), bezier_points_.size());

	MoveToEx(dc, bezier_points_[0].x, bezier_points_[0].y, nullptr);
	LineTo(dc, bezier_points_[1].x, bezier_points_[1].y);


	MoveToEx(dc, bezier_points_[2].x, bezier_points_[2].y, nullptr);
	LineTo(dc, bezier_points_[3].x, bezier_points_[3].y);
}
#pragma endregion bezier

#pragma region polygon
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
#pragma endregion polygon

#pragma region clover
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
#pragma endregion clover
#pragma endregion draw_resize_methods