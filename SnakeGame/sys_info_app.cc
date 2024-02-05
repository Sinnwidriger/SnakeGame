#include "pch.h"
#include "sys_info_app.h"

namespace applications
{

	void SysInfoApp::SetContentType(SysInfoAppContentType content_type)
	{
		display_information_ = content_type_map_[content_type];
		InvalidateRect(wnd_, nullptr, TRUE);
	}

	SysInfoApp::SysInfoApp() :
		shared::Window(L"System Information Application", WS_VSCROLL | WS_HSCROLL),
		char_dimensions_(CharDimensions())
	{
		AddMessageCallback(WM_CREATE, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandleCreate));
		AddMessageCallback(WM_SIZE, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandleSize));
		AddMessageCallback(WM_VSCROLL, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandleVertScroll));
		AddMessageCallback(WM_HSCROLL, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandleHorzScroll));
		AddMessageCallback(WM_PAINT, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandlePaint));
		AddMessageCallback(WM_DESTROY, static_cast<shared::MessageCallbackFunction>(&SysInfoApp::HandleDestroy));

		content_type_map_[SysInfoAppContentType::kSystemMetrics] = system_metrics_;
		content_type_map_[SysInfoAppContentType::kDeviceCapabilities] = device_capabilities_;
	}

	LRESULT SysInfoApp::HandleCreate(shared::MessageProcParameters mpp)
	{
		InitializeCharDimensions();
		InitializeSystemMetricValues();
		InitializeDeviceCapabilityValues();

		return 0;
	}

	LRESULT SysInfoApp::HandleSize(shared::MessageProcParameters mpp)
	{
		auto [wnd, msg, wparam, lparam] = mpp;

		int client_area_width = LOWORD(lparam);
		int client_area_height = HIWORD(lparam);

		SCROLLINFO siVert = {
			.cbSize = sizeof(SCROLLINFO),
			.fMask = SIF_ALL,
			.nMin = 0,
			.nMax = static_cast<int>(display_information_.size()),
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

	LRESULT SysInfoApp::HandleVertScroll(shared::MessageProcParameters mpp)
	{
		return HandleScroll(mpp, SB_VERT);
	}

	LRESULT SysInfoApp::HandleHorzScroll(shared::MessageProcParameters mpp)
	{
		return HandleScroll(mpp, SB_HORZ);
	}

	LRESULT SysInfoApp::HandleScroll(shared::MessageProcParameters mpp, int axis)
	{
		auto [wnd, msg, wparam, lparam] = mpp;

		SCROLLINFO si = {
			.cbSize = sizeof(SCROLLINFO),
			.fMask = SIF_ALL
		};

		// Get current scroll state
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
			ScrollWindow(wnd_, 0, char_dimensions_.height * (prev_position - si.nPos), nullptr, nullptr);
		else if (axis == SB_HORZ)
			ScrollWindow(wnd_, char_dimensions_.lower_case_width * (prev_position - si.nPos), 0, nullptr, nullptr);

		return 0;
	}

	LRESULT SysInfoApp::HandlePaint(shared::MessageProcParameters mpp)
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

		for (int i = 0; i < display_information_.size(); ++i)
		{
			DrawSystemInformation(
				dc,
				(1 - horizontal_scroll_position) * char_dimensions_.lower_case_width,
				(i - vertical_scroll_position) * char_dimensions_.height,
				*display_information_[i]
			);
		}

		EndPaint(wnd_, &ps);
		return 0;
	}

	LRESULT SysInfoApp::HandleDestroy(shared::MessageProcParameters mpp)
	{
		PostQuitMessage(0);
		return 0;
	}

	void SysInfoApp::InitializeCharDimensions()
	{
		HDC dc = GetDC(wnd_);
		TEXTMETRIC tm;
		GetTextMetrics(dc, &tm);

		char_dimensions_.lower_case_width = tm.tmAveCharWidth;
		char_dimensions_.upper_case_width = static_cast<unsigned int>(tm.tmAveCharWidth * (tm.tmPitchAndFamily & 1 ? 1.5 : 1));
		char_dimensions_.height = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(wnd_, dc);
	}

	void SysInfoApp::InitializeSystemMetricValues()
	{
		for (auto& system_information_pointer : system_metrics_)
		{
			auto system_metric_pointer = std::static_pointer_cast<sys_info_types::SystemMetric>(system_information_pointer);
			system_metric_pointer->InitializeNumericValue();
		}
	}

	void SysInfoApp::InitializeDeviceCapabilityValues()
	{
		HDC dc = GetDC(wnd_);
		for (auto& system_information_pointer : device_capabilities_)
		{
			auto device_capability_pointer = std::static_pointer_cast<sys_info_types::DeviceCapability>(system_information_pointer);
			device_capability_pointer->InitializeNumericValue(dc);
		}
		ReleaseDC(wnd_, dc);
	}


	void SysInfoApp::DrawSystemInformation(HDC dc, int x, int y, sys_info_types::SystemInformation& system_information) const
	{
		int x1, x2, x3;
		x1 = x;
		x2 = x1 + kFirstColumnCharacters * char_dimensions_.upper_case_width;
		x3 = x2 + kSecondColumnCharacters * char_dimensions_.lower_case_width;

		std::wstring column1 = system_information.name();
		std::wstring column2 = system_information.description();
		std::wstring column3 = system_information.numeric_value();

		SetTextAlign(dc, TA_LEFT | TA_TOP);

		TextOut(
			dc, x1, y,
			column1.c_str(), static_cast<int>(column1.size()));
		TextOut(
			dc, x2, y,
			column2.c_str(), static_cast<int>(column2.size()));

		SetTextAlign(dc, TA_RIGHT | TA_TOP);

		TextOut(
			dc, x3, y,
			column3.c_str(), static_cast<int>(column3.size()));
	
		SetTextAlign(dc, TA_LEFT | TA_TOP);
	}

}
