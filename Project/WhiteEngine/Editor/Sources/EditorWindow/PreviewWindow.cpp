#include "PreviewWindow.hpp"
#include "Core/Logger.hpp"
#include "glm/ext.hpp"
#include "Graphic/GLRenderer.h"

namespace Tools
{

	void PreviewWindow::Init()
	{
		m_framebuffer.Init(m_width, m_height);
		m_shader = new Graphic::Shader("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		m_textureLoaded = false;
		m_camera = new Graphic::CameraObject(m_width, m_height);
		m_windowFlags |= ImGuiWindowFlags_NoResize;
		m_bgColor = glm::vec4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
		//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
		//m_windowFlags &= ImGuiWindowFlags_NoMove;
	}

	void PreviewWindow::OnRender()
	{
		// Get the current cursor position (where your window is)
		ImVec2 pos = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddImage((void *)m_framebuffer.m_texID,
			pos, 
			ImVec2(pos.x + m_width - 10, pos.y + m_height - 30), 
			ImVec2(0, 1), ImVec2(1, 0));
		
	}

	void PreviewWindow::PreRender()
	{
		m_framebuffer.BindFrameBuffer();
		glViewport(0, 0, m_height, m_height);
		glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_entityToRender)//(m_mesh && m_mesh->IsTextureLoaded())
		{
			glm::mat4 camMatrix = m_camera->GetViewProjMatrix();
			//std::cout << "Camera Matrix: \n" << glm::to_string(camMatrix) << std::endl;
			m_shader->use();
			m_entityToRender->RenderGameObject(camMatrix);
			m_shader->unUse();
		}
		glViewport(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight()); //Restore viewport
		m_framebuffer.UnBindFrameBuffer();
	}

	void PreviewWindow::PostRender()
	{

	}

	void PreviewWindow::Terminate()
	{

	}

	bool PreviewWindow::SetMesh(EditorComponent* mesh)
	{
		if (mesh)
		{
			m_mesh = dynamic_cast<MeshRendererEC*>(mesh);

			if (m_mesh)
				return true;
		}

		return false;
	}

	void PreviewWindow::SetEntity(EditorEntity* ent)
	{
		m_entityToRender = ent;
	}

}


