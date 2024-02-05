#include "pch.h"
#include "sys_info_app.h"
#include "basic_drawing_app.h"

enum class LaunchAppType
{
	kSysInfoApp = 0,
	kBasicDrawingApp = 1
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	LaunchAppType lat = LaunchAppType::kBasicDrawingApp;
	std::shared_ptr<shared::Window> app;

	switch (lat)
	{
		case LaunchAppType::kSysInfoApp:
		{
			app = applications::SysInfoApp::Create<applications::SysInfoApp>();
			auto sys_info_app_pointer = std::static_pointer_cast<applications::SysInfoApp>(app);
			sys_info_app_pointer->SetContentType(applications::SysInfoAppContentType::kDeviceCapabilities);
			break;
		}
		case LaunchAppType::kBasicDrawingApp:
		{
			app = applications::BasicDrawingApp::Create<applications::BasicDrawingApp>();
			auto basic_draw_app_pointer = std::static_pointer_cast<applications::BasicDrawingApp>(app);
			basic_draw_app_pointer->SetContentType(applications::DrawAppContentType::kDrawBezier);
			break;
		}
	}
	
	if (!app)
	{
		MessageBox(nullptr, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	return app->Run();
}