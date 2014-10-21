
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")

#include "Application.hpp"

int main()
{
	Application app(1280u, 720u, "OpenGL 2D");

	app.run();

	return 0;
}