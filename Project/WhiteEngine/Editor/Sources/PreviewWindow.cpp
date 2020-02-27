#include "PreviewWindow.hpp"

namespace Tools
{
	PreviewWindow::PreviewWindow() : UIWindow("Preview", 800, 800)
	{
		m_shader = new Graphic::Shader("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		m_framebuffer.Init(m_width, m_height);
	}

	PreviewWindow::~PreviewWindow()
	{

	}

	void PreviewWindow::Init()
	{
		m_textureLoaded = false;
		m_windowFlags |= ImGuiWindowFlags_NoCollapse;
		//ImGui::SetNextWindowSize(ImVec2(m_height + 10, m_height + 10), ImGuiCond_FirstUseEver);
	}

	void PreviewWindow::OnRender()
	{
		// Get the current cursor position (where your window is)
		ImVec2 pos = ImGui::GetCursorScreenPos();

		if (m_textureLoaded)
		{
			//Render object in to framebuffer
			m_framebuffer.BindFrameBuffer();
			m_shader->use();


			m_shader->unUse();
			m_framebuffer.UnBindFrameBuffer();


			ImGui::GetWindowDrawList()->AddImage((void *)m_framebuffer.m_fboID, 
				ImVec2(ImGui::GetItemRectMin().x - pos.x, ImGui::GetItemRectMin().y + pos.y)
				, ImVec2(pos.x + m_height / 2, pos.y + m_width / 2), ImVec2(0, 1), ImVec2(1, 0));
			
			
		}
			
	}

	

	void PreviewWindow::Terminate()
	{

	}

	bool PreviewWindow::SetTexture(unsigned int textureID)
	{
		m_textureID = textureID;
		m_textureLoaded = true;
		return true;
	}

}


