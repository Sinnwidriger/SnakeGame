#include "stdafx.h"
#include "sys_info_app/sys_info_app.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	auto app = shared::Window::Create<SysInfoApp>();
	app->SetContentType(SysInfoAppContent::kDeviceCapabilities);

	if (!app)
	{
		MessageBox(nullptr, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}


	return app->Run();
}