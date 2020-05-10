#include "GameplayWindow.hpp"
#include "Graphic/GLRenderer.h"
#include "Graphic/GraphicCore.hpp"



namespace Tools
{
	void GameplayWindow::Init()
	{
		m_windowFlags |= ImGuiWindowFlags_NoResize;
	}

	void GameplayWindow::PreRender()
	{

	}

	void GameplayWindow::OnRender()
	{
		
		auto tex = GLRenderer::GetInstance()->GetFrameBuffer();

		// Get the current cursor position (where your window is)
		ImVec2 pos = ImGui::GetCursorScreenPos();

		// A boolean to allow me to stop the game rendering
		if (m_state == GP_STATE::RUN) 
		{
			Graphic::g_renderer->SetViewPort(0, 0, m_width, m_height);
			// Update scene
			//TODO: Update Animator
			//TODO: Update MeshRender
			// Render the scene into an FBO
			
			//Graphic::Render();
			Graphic::g_renderer->SetViewPort(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		}
		// Get the texture associated to the FBO
			

		// Ask ImGui to draw it as an image:
		// Under OpenGL the ImGUI image type is GLuint
		// So make sure to use "(void *)tex" but not "&tex"
		ImGui::GetWindowDrawList()->AddImage(
		(void *)tex->m_texID, 
			pos, ImVec2(pos.x + m_width - 10, pos.y + m_height - 30), ImVec2(0, 1), ImVec2(1, 0));
	}

	void GameplayWindow::Terminate()
	{

	}

	void GameplayWindow::RunGame()
	{
		//Reload Scene or Reset Scene?

		//Set state
		m_state = GP_STATE::RUN;
	}

	void GameplayWindow::PauseGame()
	{
		m_state = GP_STATE::PAUSE;
	}

	void GameplayWindow::StopGame()
	{
		m_state = GP_STATE::STOP;
	}


}


