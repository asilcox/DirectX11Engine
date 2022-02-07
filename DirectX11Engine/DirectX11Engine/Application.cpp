#include "Application.h"
#include "imgui/imgui.h"

Application::Application()
	:
	window(1280, 720, "DirectX Tutorials"),
	renderManager(window.GetHWND()),
	cube(renderManager)
{
	t =
	{
		{ 0.0f, 0.0f, 5.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f }
	};
}

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
	renderManager.ClearBuffer(0.0f, 0.1f, 0.2f);

	ImGui::Begin("Cube");
	ImGui::SliderFloat3("Position", &t.pos.x, 0.0f, 5.0f);
	ImGui::SliderFloat3("Rotation", &t.rot.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Scaling", &t.scale.x, 0.0f, 5.0f);
	ImGui::End();

	cube.SetTransform(t);
	cube.Update(renderManager);

	renderManager.DrawIndexedMesh(&cube);
	renderManager.EndFrame();
}