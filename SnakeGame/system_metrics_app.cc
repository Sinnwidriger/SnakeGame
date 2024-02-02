#include "pch.h"
#include "system_metrics_app.h"

namespace snake_game
{

std::unique_ptr<SystemMetricsApp> SystemMetricsApp::Create()
{
	auto app_pointer = std::unique_ptr<SystemMetricsApp>(new SystemMetricsApp);

	if (app_pointer->Initialize())
		return app_pointer;
	else
		return nullptr;
}

SystemMetricsApp::SystemMetricsApp() :
	instance_(GetModuleHandle(NULL)), wnd_(NULL), window_class_name_(L"SystemMetrics"), char_dimensions_(CharDimensions())
{ }

bool SystemMetricsApp::Initialize()
{
	WNDCLASS wc = {
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = WindowProc,
		.hInstance = instance_,
		.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
		.lpszClassName = window_class_name_.c_str()
	};

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"Window Registration failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	wnd_ = CreateWindow(
		window_class_name_.c_str(),
		L"SystemMetrics Application",
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		NULL, NULL,
		instance_,
		this
	);

	if (!wnd_)
	{
		MessageBox(NULL, L"Window Creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	ShowWindow(wnd_, SW_SHOWNORMAL);
	UpdateWindow(wnd_);

	return true;
}

int SystemMetricsApp::Run()
{
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

LRESULT SystemMetricsApp::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	SystemMetricsApp* app_pointer = nullptr;

	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* create_struct_pointer = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_pointer = static_cast<SystemMetricsApp*>(create_struct_pointer->lpCreateParams);
		SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app_pointer));
	}
	else
	{
		app_pointer = reinterpret_cast<SystemMetricsApp*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
	}

	if (app_pointer)
	{
		MessageProcParameters mpp = {
			.wnd = wnd,
			.msg = msg,
			.wparam = wparam,
			.lparam = lparam
		};
		return app_pointer->HandleMessage(mpp);
	}
	else
	{
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}

LRESULT SystemMetricsApp::HandleMessage(MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	switch (msg)
	{
	case WM_CREATE:
		return HandleCreate(mpp);
	case WM_SIZE:
		return HandleSize(mpp);
	case WM_VSCROLL:
		return HandleScroll(mpp, SB_VERT);
	case WM_HSCROLL:
		return HandleScroll(mpp, SB_HORZ);
	case WM_PAINT:
		return HandlePaint(mpp);
	case WM_DESTROY:
		return HandleDestroy(mpp);
	default:
		return DefWindowProc(wnd, msg, wparam, lparam);
	}
}

LRESULT SystemMetricsApp::HandleCreate(MessageProcParameters mpp)
{
	HDC dc = GetDC(wnd_);

	TEXTMETRIC tm;
	GetTextMetrics(dc, &tm);

	char_dimensions_.lower_case_width = tm.tmAveCharWidth;
	char_dimensions_.upper_case_width = static_cast<unsigned int>(tm.tmAveCharWidth * (tm.tmPitchAndFamily & 1 ? 1.5 : 1));
	char_dimensions_.height = tm.tmHeight + tm.tmExternalLeading;

	ReleaseDC(wnd_, dc);

	return 0;
}

LRESULT SystemMetricsApp::HandleSize(MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	int client_area_width = LOWORD(lparam);
	int client_area_height = HIWORD(lparam);

	SCROLLINFO siVert = {
		.cbSize = sizeof(SCROLLINFO),
		.fMask = SIF_ALL,
		.nMin = 0,
		.nMax = static_cast<int>(sm_entries_.size()),
		.nPage = static_cast<unsigned int>(client_area_height / char_dimensions_.height)
	};
	SetScrollInfo(wnd_, SB_VERT, &siVert, TRUE);

	SCROLLINFO siHorz = {
		.cbSize = sizeof(SCROLLINFO),
		.fMask = SIF_ALL,
		.nMin = 0,
		// Multiply "kFirstColumnCharacters" by "1.5" because characters in first column CAPITALIZED
		.nMax = static_cast<int>(kFirstColumnCharacters * 1.5 + kSecondColumnCharacters),
		.nPage = static_cast<unsigned int>(client_area_width / char_dimensions_.lower_case_width)
	};
	SetScrollInfo(wnd_, SB_HORZ, &siHorz, TRUE);

	return 0;
}

LRESULT SystemMetricsApp::HandleScroll(MessageProcParameters mpp, int axis)
{
	auto [wnd, msg, wparam, lparam] = mpp;

	SCROLLINFO si = {
		.cbSize = sizeof(SCROLLINFO),
		.fMask = SIF_ALL
	};

	// Get current vertical scroll state
	GetScrollInfo(wnd_, axis, &si);
	int prev_position = si.nPos;

	// Update position according to recieved command
	switch (LOWORD(wparam))
	{
	case SB_TOP | SB_LEFT:
		si.nPos = si.nMin;
		break;
	case SB_BOTTOM | SB_RIGHT:
		si.nPos = si.nMax;
		break;
	case SB_LINEUP | SB_LINELEFT:
		si.nPos -= 1;
		if (si.nPos < si.nMin) si.nPos = si.nMin;
		break;
	case SB_LINEDOWN | SB_LINERIGHT:
		si.nPos += 1;
		if (si.nPos > si.nMax - static_cast<int>(si.nPage)) si.nPos = si.nMax - static_cast<int>(si.nPage);
		break;
	case SB_PAGEUP | SB_PAGELEFT:
		si.nPos -= si.nPage;
		break;
	case SB_PAGEDOWN | SB_PAGERIGHT:
		si.nPos += si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	}

	// Set updated vertical scroll state
	SetScrollInfo(wnd_, axis, &si, TRUE);
	// Update window according to scrolled distance
	if (axis == SB_VERT)
		ScrollWindow(wnd_, 0, char_dimensions_.height * (prev_position - si.nPos), NULL, NULL);
	else if (axis == SB_HORZ)
		ScrollWindow(wnd_, char_dimensions_.lower_case_width * (prev_position - si.nPos), 0, NULL, NULL);

	return 0;
}

LRESULT SystemMetricsApp::HandlePaint(MessageProcParameters mpp)
{

	PAINTSTRUCT ps;
	HDC dc = BeginPaint(wnd_, &ps);

	SCROLLINFO vertical_scroll_info = {
		.cbSize = sizeof(SCROLLINFO),
		.fMask = SIF_POS
	};
	GetScrollInfo(wnd_, SB_VERT, &vertical_scroll_info);
	int vertical_scroll_position = vertical_scroll_info.nPos;

	SCROLLINFO horizontal_scroll_info = {
		.cbSize = sizeof(SCROLLINFO),
		.fMask = SIF_POS
	};
	GetScrollInfo(wnd_, SB_HORZ, &horizontal_scroll_info);
	int horizontal_scroll_position = horizontal_scroll_info.nPos;

	for (int i = 0; i < sm_entries_.size(); ++i)
	{
		DrawSystemMetric(
			dc,
			(1 - horizontal_scroll_position) * char_dimensions_.lower_case_width,
			(i - vertical_scroll_position) * char_dimensions_.height,
			sm_entries_[i]
		);
	}

	EndPaint(wnd_, &ps);
	return 0;
}

LRESULT SystemMetricsApp::HandleDestroy(MessageProcParameters mpp)
{
	PostQuitMessage(0);
	return 0;
}

void SystemMetricsApp::DrawSystemMetric(HDC dc, int x, int y, SystemMetric& sm_entry) const
{
	auto& [sm_index, sm_name, sm_description, sm_numeric_value] = sm_entry;

	int x1 = x, x2 = x, x3 = x;
	int y1 = y, y2 = y, y3 = y;
	x2 = x1 + kFirstColumnCharacters * char_dimensions_.upper_case_width;
	x3 = x2 + kSecondColumnCharacters * char_dimensions_.lower_case_width;

	sm_numeric_value = std::to_wstring(GetSystemMetrics(sm_index));

	SetTextAlign(dc, TA_LEFT | TA_TOP);
	TextOut(
		dc, x1, y1,
		sm_name.c_str(), static_cast<int>(sm_name.size())
	);
	TextOut(
		dc, x2, y2,
		sm_description.c_str(), static_cast<int>(sm_description.size())
	);

	SetTextAlign(dc, TA_RIGHT | TA_TOP);
	TextOut(
		dc, x3, y3,
		sm_numeric_value.c_str(), static_cast<int>(sm_numeric_value.size())
	);
	
	SetTextAlign(dc, TA_LEFT | TA_TOP);
}

}
