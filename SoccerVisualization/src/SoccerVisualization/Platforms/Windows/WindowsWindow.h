#pragma once


#include "SoccerVisualization/Core/Window.h"
#include "SoccerVisualization/Platforms/OpenGL/OpenGLContext.h"

namespace SoccerCamRecorder {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Hazel Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};
	class WindowsWindow
	{

	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void OnUpdate();

		unsigned int GetWidth() const  { return m_Data.Width; }
		unsigned int GetHeight() const  { return m_Data.Height; }

		// Window attributes
		void SetVSync(bool enabled) ;
		bool IsVSync() const;

		virtual void* GetNativeWindow() const { return m_Window; }

		static Scope<WindowsWindow> Create(const WindowProps& props = WindowProps());
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<OpenGLContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};
}