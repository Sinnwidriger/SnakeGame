#include "stdafx.h"
#include "drawing_app.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	auto app = shared::Window::Create<DrawingApp>();
	app->SetContentType(DrawAppContentType::kDrawClover);

	if (!app)
	{
		MessageBox(nullptr, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}


	return app->Run();
}