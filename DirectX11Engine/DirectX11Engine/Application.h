#pragma once
#include "Window.h"

class Application
{
public:
	Application();
	~Application();
	int Run();
	void DrawFrame();
private:
	Window window;
};