#pragma once
struct GLFWwindow;

namespace SoccerCamRecorder{

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		void Init();
		void SwapBuffers();

		static Scope<OpenGLContext> Create(GLFWwindow* window);
	private:
		GLFWwindow* m_WindowHandle;
	};

}
