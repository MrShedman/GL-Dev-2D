
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")

#include "Application.hpp"

int main()
{
	ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 2;
	settings.core = true;

	Window window(VideoMode(1280, 720), "OpenGL 2D", Style::Close, settings);

	Application app(window);

	app.run();

	return 0;
}