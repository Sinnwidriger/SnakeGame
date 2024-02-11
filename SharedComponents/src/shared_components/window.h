#pragma once

#ifdef SHAREDCOMPONENTS_EXPORTS
#define SHAREDCOMPONENTS_API __declspec(dllexport)
#else
#define SHAREDCOMPONENTS_API __declspec(dllimport)
#endif

namespace shared
{
	class SHAREDCOMPONENTS_API Window;
	template<typename T>
	concept WindowConcept = std::is_base_of<Window, T>::value;

	struct MessageProcParameters {
		HWND wnd;
		UINT msg;
		WPARAM wparam;
		LPARAM lparam;
	};

	using MessageCallback = std::function<LRESULT(MessageProcParameters)>;
	using MessageMethodPtr = LRESULT(Window::*)(MessageProcParameters);

	class SHAREDCOMPONENTS_API Window
	{
		// Constructors
	protected:
		// Clients can't invoke the constructor directly
		Window();
		Window(const std::wstring& window_title, DWORD window_style = WS_OVERLAPPEDWINDOW);

	public:
		// This class is NOT copyable BUT moveable
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
		virtual ~Window() = default;

		// Static Methods
	public:
		template<WindowConcept T>
		static std::shared_ptr<T> Create();
		static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static MessageCallback MessageMethodToCallback(Window* window_instance, MessageMethodPtr method_ptr);

		// Methods
	public:
		bool Initialize();
		int Run();

		// Methods
	protected:
		void AddMessageCallback(UINT listen_msg, MessageMethodPtr method_ptr);

	private:
		LRESULT HandleMessage(MessageProcParameters mpp);
		LRESULT HandleSize(MessageProcParameters mpp);
		virtual void Idle();


		// Fields
	protected:
		HINSTANCE instance_;
		HWND wnd_;
		std::wstring window_title_;
		std::wstring window_class_name_;
		DWORD window_style_;
		int client_width_;
		int client_height_;

	private:
		std::unordered_map<UINT, std::vector<MessageCallback>> listeners_;
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