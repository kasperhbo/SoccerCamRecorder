#include "scrpch.h"
#include "Window.h"

#include "SoccerVisualization/Platforms/Windows/WindowsWindow.h"

namespace SoccerCamRecorder {
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
}