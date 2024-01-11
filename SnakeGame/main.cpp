#include "pch.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	MessageBoxW(NULL, TEXT("Hello, world!"), TEXT("MessageBox"), MB_ICONQUESTION);
}