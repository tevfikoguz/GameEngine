#include <Utility\Window.h>
#include <Platform\PlatformWindow.h>

using namespace Engine::Utility;
using namespace Engine::Platform;

void main()
{
	Window window("Test");
	window.Initialize();
	window.SetVisible(true);

	while (!window.ShouldClose())
	{
		PlatformWindow::PollEvents();
	}
}