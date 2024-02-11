#include "stdafx.h"
#include "shared_components/window.h"
#include "drawing_app/drawing_app.h"

//int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
//{
//	auto app = shared::Window::Create<DrawingApp>();
//	app->SetContentType(DrawAppContent::kDrawNothing);
//
//	if (!app)
//	{
//		MessageBox(nullptr, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
//		return -1;
//	}
//
//
//	return app->Run();
//}

int main()
{
	int width = 11;
	for (int i = 1; i <= width; i += 2)
	{
		std::string pyramid_line(width, ' ');
		std::string pyramid_block(i, '*');
		int mid = (width - i) / 2;
		pyramid_line.replace(mid, i, pyramid_block);
		std::cout << pyramid_line << std::endl;
	}
}