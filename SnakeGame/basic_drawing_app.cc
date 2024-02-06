#include "pch.h"
#include "basic_drawing_app.h"

namespace applications
{
	void BasicDrawingApp::SetContentType(DrawAppContentType content_type)
	{
		draw_content_type_ = content_type;
		InvalidateRect(wnd_, nullptr, TRUE);
	}

	BasicDrawingApp::BasicDrawingApp() :
		shared::Window(L"Basic Drawing Application")
	{
		AddMessageCallback(WM_SIZE, static_cast<shared::MessageCallbackFunction>(&BasicDrawingApp::HandleSize));
		AddMessageCallback(WM_PAINT, static_cast<shared::MessageCallbackFunction>(&BasicDrawingApp::HandlePaint));
		AddMessageCallback(WM_DESTROY, static_cast<shared::MessageCallbackFunction>(&BasicDrawingApp::HandleDestroy));

		draw_function_callback_map_[DrawAppContentType::kDrawSinWave] = GetBindedDrawFunctionCallback(&BasicDrawingApp::DrawSinWave);
		draw_function_callback_map_[DrawAppContentType::kDrawLines] = GetBindedDrawFunctionCallback(&BasicDrawingApp::DrawLines);
		draw_function_callback_map_[DrawAppContentType::kDrawBezier] = GetBindedDrawFunctionCallback(&BasicDrawingApp::DrawBezier);
		draw_function_callback_map_[DrawAppContentType::kDrawPolygon] = GetBindedDrawFunctionCallback(&BasicDrawingApp::DrawPolygon);
	}

	LRESULT BasicDrawingApp::HandleSize(shared::MessageProcParameters mpp)
	{
		auto [wnd, msg, wparam, lparam] = mpp;

		int client_area_width = LOWORD(lparam);
		int client_area_height = HIWORD(lparam);

		InitializeSinWavePoints(client_area_width, client_area_height);
		InitializeBezierPoints(client_area_width, client_area_height);
		InitializePolygonPoints(client_area_width, client_area_height);

		return 0;
	}

	LRESULT BasicDrawingApp::HandleMouse(shared::MessageProcParameters mpp)
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

	LRESULT BasicDrawingApp::HandlePaint(shared::MessageProcParameters mpp)
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(wnd_, &ps);

		draw_function_callback_map_[draw_content_type_](dc, ps);

		EndPaint(wnd_, &ps);

		return 0;
	}

	LRESULT BasicDrawingApp::HandleDestroy(shared::MessageProcParameters mpp)
	{
		PostQuitMessage(0);
		return 0;
	}

	auto BasicDrawingApp::GetBindedDrawFunctionCallback(DrawFunctionCallback callback) -> std::function<void(HDC, PAINTSTRUCT)>
	{
		return std::bind(callback, this, std::placeholders::_1, std::placeholders::_2);
	}

	void BasicDrawingApp::InitializeSinWavePoints(int client_area_width, int client_area_height)
	{
		sin_wave_points_.clear();
		for (int i = 0; i < 1000; ++i)
		{
			POINT pt = {
				.x = i * client_area_width / 1000,
				.y = static_cast<int>(client_area_height / 2 * (1 - std::sin(2 * std::numbers::pi * i / 1000)))
			};

			sin_wave_points_.push_back(pt);
		}
	}

	void BasicDrawingApp::InitializeBezierPoints(int client_area_width, int client_area_height)
	{
		bezier_points_.clear();

		bezier_points_.push_back({ client_area_width / 4, client_area_height / 2 });
		bezier_points_.push_back({ client_area_width / 2, client_area_height / 4 });
		bezier_points_.push_back({ client_area_width / 2, client_area_height * 3 / 4 });
		bezier_points_.push_back({ client_area_width * 3 / 4, client_area_height / 2 });
	}

	void BasicDrawingApp::InitializePolygonPoints(int client_area_width, int client_area_height)
	{
		polygon_points_.clear();

		polygon_points_.push_back({ 10 * client_area_width / 200, 70 * client_area_height / 100 });
		polygon_points_.push_back({ 50 * client_area_width / 200, 70 * client_area_height / 100 });
		polygon_points_.push_back({ 50 * client_area_width / 200, 10 * client_area_height / 100 });
		polygon_points_.push_back({ 90 * client_area_width / 200, 10 * client_area_height / 100 });
		polygon_points_.push_back({ 90 * client_area_width / 200, 50 * client_area_height / 100 });
		polygon_points_.push_back({ 30 * client_area_width / 200, 50 * client_area_height / 100 });
		polygon_points_.push_back({ 30 * client_area_width / 200, 90 * client_area_height / 100 });
		polygon_points_.push_back({ 70 * client_area_width / 200, 90 * client_area_height / 100 });
		polygon_points_.push_back({ 70 * client_area_width / 200, 30 * client_area_height / 100 });
		polygon_points_.push_back({ 10 * client_area_width / 200, 30 * client_area_height / 100 });
	}

	void BasicDrawingApp::DrawSinWave(HDC dc, PAINTSTRUCT ps)
	{
		MoveToEx(dc, 0, ps.rcPaint.bottom / 2, nullptr);
		LineTo(dc, ps.rcPaint.right, ps.rcPaint.bottom / 2);
		Polyline(dc, sin_wave_points_.data(), static_cast<int>(sin_wave_points_.size()));
	}

	void BasicDrawingApp::DrawLines(HDC dc, PAINTSTRUCT ps)
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

	void BasicDrawingApp::DrawBezier(HDC dc, PAINTSTRUCT ps)
	{
		PolyBezier(dc, bezier_points_.data(), bezier_points_.size());

		MoveToEx(dc, bezier_points_[0].x, bezier_points_[0].y, nullptr);
		LineTo(dc, bezier_points_[1].x, bezier_points_[1].y);


		MoveToEx(dc, bezier_points_[2].x, bezier_points_[2].y, nullptr);
		LineTo(dc, bezier_points_[3].x, bezier_points_[3].y);
	}

	void BasicDrawingApp::DrawPolygon(HDC dc, PAINTSTRUCT ps)
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

}