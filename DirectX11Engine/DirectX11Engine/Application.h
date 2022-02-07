#pragma once
#include "Window.h"
#include "RenderManager.h"
#include "Cube.h"

class Application
{
public:
	Application();
	~Application();
	int Run();
	void DrawFrame();
private:
	Window window;
	RenderManager renderManager;
	Transform t;
	Cube cube;
};