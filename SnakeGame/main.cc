#include "pch.h"
#include "sys_info_app.h"
#include "basic_drawing_app.h"
#include "what_size_app.h"

enum class LaunchAppType
{
	kSysInfoApp = 0,
	kBasicDrawingApp = 1,
	kWhatSizeApp = 2
};

std::shared_ptr<shared::Window> GetSysInfoApp()
{
	auto app = applications::SysInfoApp::Create<applications::SysInfoApp>();
	app->SetContentType(applications::SysInfoAppContentType::kSystemMetrics);

	return app;
}

std::shared_ptr<shared::Window> GetBasicDrawingApp()
{
	auto app = applications::BasicDrawingApp::Create<applications::BasicDrawingApp>();
	app->SetContentType(applications::DrawAppContentType::kDrawPolygon);

	return app;
}

std::shared_ptr<shared::Window> GetWhatSizeApp()
{
	auto app = applications::WhatSizeApp::Create<applications::WhatSizeApp>();

	return app;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	std::unordered_map<LaunchAppType, std::function<std::shared_ptr<shared::Window>()>> map;

	LaunchAppType lat = LaunchAppType::kWhatSizeApp;

	map[LaunchAppType::kSysInfoApp] = &GetSysInfoApp;
	map[LaunchAppType::kBasicDrawingApp] = &GetBasicDrawingApp;
	map[LaunchAppType::kWhatSizeApp] = &GetWhatSizeApp;

	auto app = map[lat]();
	if (!app)
	{
		MessageBox(nullptr, L"App creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	return app->Run();
}