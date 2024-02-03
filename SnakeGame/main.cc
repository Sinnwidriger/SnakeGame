#include "pch.h"
#include "system_information_app.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	auto app = applications::SystemInformationApp::Create();
	if (!app)
	{
		MessageBox(NULL, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	return app->Run();
}