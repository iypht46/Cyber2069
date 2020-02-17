#include "PreviewWindow.hpp"
#include "Graphic/GLRenderer.h"
#include "Graphic/GraphicCore.hpp"


namespace Tools
{
	void PreviewWindow::Init()
	{
		Graphic::EnableFrameBuffer(FBO_STATE::SUB);
	}

	void PreviewWindow::OnRender()
	{
		auto tex = Graphic::g_renderer->GetFrameBuffer();
		ImGui::SetNextWindowSize(ImVec2(m_height + 10, m_height + 10), ImGuiCond_FirstUseEver);

		// Get the current cursor position (where your window is)
		ImVec2 pos = ImGui::GetCursorScreenPos();

		// A boolean to allow me to stop the game rendering
		if (m_run) 
		{
			Graphic::g_renderer->SetViewPort(0, 0, m_width, m_height);
			// Render the scene into an FBO
			Graphic::Render();
			Graphic::g_renderer->SetViewPort(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		}
		// Get the texture associated to the FBO
			

		// Ask ImGui to draw it as an image:
		// Under OpenGL the ImGUI image type is GLuint
		// So make sure to use "(void *)tex" but not "&tex"
		ImGui::GetWindowDrawList()->AddImage(
		(void *)tex, ImVec2(ImGui::GetItemRectMin().x - pos.x,
		ImGui::GetItemRectMin().y + pos.y),
		ImVec2(pos.x + m_height / 2, pos.y + m_width / 2), ImVec2(0, 1), ImVec2(1, 0));
	}

	void PreviewWindow::Terminate()
	{
	}
}


