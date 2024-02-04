#include "pch.h"
#include "window.h"

namespace shared
{

bool shared::Window::Initialize()
{
	WNDCLASS wc = {
		 .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		 .lpfnWndProc = WindowProc,
		 .hInstance = instance_,
		 .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
		 .lpszClassName = window_class_name_.c_str()
	};

	if (!RegisterClass(&wc))
	{
		MessageBox(nullptr, L"Window Registration failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	wnd_ = CreateWindow(
		window_class_name_.c_str(),
		window_title_.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		nullptr, nullptr,
		instance_,
		this
	);

	if (!wnd_)
	{
		MessageBox(nullptr, L"Window Creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	ShowWindow(wnd_, SW_SHOWNORMAL);
	UpdateWindow(wnd_);

	return true;
}

int shared::Window::Run()
{
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
}

Window::Window() :
	Window(L"Default Window")
{ }

Window::Window(const std::wstring& window_title) :
	instance_(GetModuleHandle(nullptr)), wnd_(nullptr), window_title_(window_title), window_class_name_(window_title + L" Class")
{ }

LRESULT Window::WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* app_pointer = nullptr;

	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* create_struct_pointer = reinterpret_cast<CREATESTRUCT*>(lparam);
		app_pointer = static_cast<Window*>(create_struct_pointer->lpCreateParams);
		SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app_pointer));
	}
	else
	{
		app_pointer = reinterpret_cast<Window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
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

LRESULT Window::HandleMessage(MessageProcParameters mpp)
{
	auto [wnd, msg, wparam, lparam] = mpp;
	return DefWindowProc(wnd, msg, wparam, lparam);
}

}


