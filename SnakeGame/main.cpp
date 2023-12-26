#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	MessageBoxW(NULL, TEXT("Hello, world!"), TEXT("MessageBox"), 0);
}