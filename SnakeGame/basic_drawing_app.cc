#include "pch.h"
#include "basic_drawing_app.h"

namespace applications
{
	void BasicDrawingApp::SetContentType(DrawAppContentType content_type)
	{
		switch (content_type)
		{
		case DrawAppContentType::kDrawSinWave:
			draw_function_callback_ = std::bind(&BasicDrawingApp::DrawSinWave, this, std::placeholders::_1, std::placeholders::_2);
			break;
		case DrawAppContentType::kDrawLines:
			draw_function_callback_ = std::bind(&BasicDrawingApp::DrawLines, this, std::placeholders::_1, std::placeholders::_2);
			break;
		}

		InvalidateRect(wnd_, nullptr, TRUE);
	}

	BasicDrawingApp::BasicDrawingApp() :
		shared::Window(L"Basic Drawing Application"),
		draw_function_callback_(std::bind(&BasicDrawingApp::DrawSinWave, this, std::placeholders::_1, std::placeholders::_2))
	{ }

LRESULT BasicDrawingApp::HandleSize(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	int client_area_width = LOWORD(lparam);
	int client_area_height = HIWORD(lparam);

	InitializeSinWavePoints(client_area_width, client_area_height);

	return 0;
}

LRESULT BasicDrawingApp::HandlePaint(shared::MessageProcParameters mpp)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(wnd_, &ps);

	draw_function_callback_(dc, ps);

	EndPaint(wnd_, &ps);

	return 0;
}

LRESULT BasicDrawingApp::HandleDestroy(shared::MessageProcParameters mpp)
{
	PostQuitMessage(0);
  return 0;
}

LRESULT BasicDrawingApp::HandleMessage(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	switch (msg)
	{
	case WM_SIZE:
		return HandleSize(mpp);
	case WM_PAINT:
		return HandlePaint(mpp);
	case WM_DESTROY:
		return HandleDestroy(mpp);
	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
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

}