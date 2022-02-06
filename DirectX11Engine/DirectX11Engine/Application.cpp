#include "Application.h"

Application::Application()
	:
	window(1280, 720, "DirectX Tutorials")
{}

Application::~Application()
{}

int Application::Run()
{
	while (true)
	{
		if (const auto msgCode = Window::ProcessMessages())
			return *msgCode;

		DrawFrame();
	}
}

void Application::DrawFrame()
{

}

