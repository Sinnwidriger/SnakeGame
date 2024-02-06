#include "pch.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	MessageBox(nullptr, L"Hello world!", L"Title", MB_OK);
	return 0;
}