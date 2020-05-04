#include "PreviewWindow.hpp"
#include "Core/Logger.hpp"
#include "glm/ext.hpp"
#include "Graphic/GLRenderer.h"
#include "Core/GameInfo.h"

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
		PreviewCameraControl();
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

		if (m_customRender && m_customRenderFunction)
		{
			glm::mat4 camMatrix = m_camera->GetViewProjMatrix();
			//std::cout << "Camera Matrix: \n" << glm::to_string(camMatrix) << std::endl;
			m_shader->use();

			m_customRenderFunction(camMatrix);

			m_shader->unUse();
		}
		else
		{
			if (m_entityToRender)//(m_mesh && m_mesh->IsTextureLoaded())
			{
				glm::mat4 camMatrix = m_camera->GetViewProjMatrix();
				//std::cout << "Camera Matrix: \n" << glm::to_string(camMatrix) << std::endl;
				m_shader->use();
				auto anim = m_entityToRender->GetGameObject()->GetComponent<Animator>();
				if (anim)
					anim->animUpdate(World::GameInfo::GetInstance().m_deltaTime);
				m_entityToRender->RenderGameObject(camMatrix);
				m_shader->unUse();
			}
		}
		

		glViewport(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight()); //Restore viewport
		m_framebuffer.UnBindFrameBuffer();
	}

	void PreviewWindow::PostRender()
	{

	}

	void PreviewWindow::SetCustomRenderFunction(RenderFunction func)
	{
		if (func)
		{
			m_customRenderFunction = func;
			m_customRender = true;
		}
	}

	void PreviewWindow::SetCustomRender(bool i)
	{
		m_customRender = i;
	}

	void PreviewWindow::PreviewCameraControl()
	{
		if (ImGui::IsWindowHovered())
		{
			ImGuiIO& io = ImGui::GetIO();
			float zoom_value = io.MouseWheel;
			glm::vec3 move_value;

			//Zoom
			if (zoom_value > 0.1f || zoom_value < 0.1f)
			{
				if (zoom_value > 1.5f || zoom_value < -1.5f)
					zoom_value *= m_camera->GetZoom() * 0.05 * -1.0f;
				else
					zoom_value *= m_camera->GetZoom() * 0.01 * -1.0f;

				m_camera->Zoom(zoom_value);
			}
		}
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


