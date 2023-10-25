#include "scrpch.h"
#include "SoccerVisualization/Platforms/OpenGL/OpenGLContext.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace SoccerCamRecorder{
	Scope<OpenGLContext> OpenGLContext::Create(GLFWwindow* window)
	{
		return CreateScope<OpenGLContext>(window);	
	}

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{	
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		CORE_INFO("OpenGL Info:");
		CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		//HZ_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		//HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}