#ifndef SNAKE_GAME_SHARED_WINDOW_H_
#define SNAKE_GAME_SHARED_WINDOW_H_

namespace shared
{

struct MessageProcParameters {
	HWND wnd;
	UINT msg;
	WPARAM wparam;
	LPARAM lparam;
};

class Window;
template<typename T>
concept WindowConcept = std::is_base_of<Window, T>::value;

class Window
{
public:
	// Constructors
	// This class is NOT copyable BUT moveable
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;


	template<WindowConcept T>
	static std::shared_ptr<T> Create();

	// Public Methods
	bool Initialize();
	int Run();

	virtual ~Window() = default;

protected:
	// Clients can't invoke the constructor directly
	Window();
	Window(const std::wstring& window_title, DWORD window_style = WS_OVERLAPPEDWINDOW);

	// Methods
	static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT HandleMessage(MessageProcParameters mpp);

	// Fields
	HINSTANCE instance_;
	HWND wnd_;
	std::wstring window_title_;
	std::wstring window_class_name_;
	DWORD window_style_;
};

template<WindowConcept T>
static std::shared_ptr<T> Window::Create()
{
	auto app_pointer = std::unique_ptr<T>(new T());

	if (app_pointer->Initialize())
		return app_pointer;
	else
		return std::unique_ptr<T>(nullptr);
}

}

#endif