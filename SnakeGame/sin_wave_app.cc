#include "pch.h"
#include "sin_wave_app.h"

namespace applications
{

	SinWaveApp::SinWaveApp() :
		shared::Window(L"Sin Wave Application"),
		client_area_width_(0), client_area_height_(0)
	{ }

LRESULT SinWaveApp::HandleSize(shared::MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	client_area_width_ = LOWORD(lparam);
	client_area_height_ = HIWORD(lparam);

	points_.clear();

	for (int i = 0; i < 1000; ++i)
	{
		POINT pt = {
			.x = i * client_area_width_ / 1000,
			.y = static_cast<int>(client_area_height_ / 2 * (1 - std::sin(2 * std::numbers::pi * i / 1000)))
		};

		points_.push_back(pt);
	}

	return 0;
}

LRESULT SinWaveApp::HandlePaint(shared::MessageProcParameters mpp)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(wnd_, &ps);

	MoveToEx(dc, 0, client_area_height_ / 2, nullptr);
	LineTo(dc, client_area_width_, client_area_height_ / 2);

	Polyline(dc, points_.data(), static_cast<int>(points_.size()));

	EndPaint(wnd_, &ps);

	return 0;
}

LRESULT SinWaveApp::HandleDestroy(shared::MessageProcParameters mpp)
{
	PostQuitMessage(0);
  return 0;
}

LRESULT SinWaveApp::HandleMessage(shared::MessageProcParameters mpp)
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

}