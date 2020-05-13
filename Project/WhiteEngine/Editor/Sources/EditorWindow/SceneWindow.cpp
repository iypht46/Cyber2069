#include "SceneWindow.hpp"

//White Engine
#include "Graphic/GLRenderer.h"
#include "Graphic/GraphicCore.hpp"
#include <iostream>
namespace Tools
{
	

	void SceneWindow::Init()
	{
		m_sceneCam = new Graphic::CameraObject(m_width, m_height);
		m_framebuffer = new Graphic::Framebuffer();//GLRenderer::GetInstance()->GetFrameBuffer();
		m_framebuffer->Init(m_width, m_height);
		//m_windowFlags |= ImGuiWindowFlags_NoResize;
	}

	void SceneWindow::OnRender()
	{
		this->SceneCameraControl();

		//Get Window Position
		ImVec2 pos = ImGui::GetCursorScreenPos();
		//Draw framebuffer on window
		ImGui::GetWindowDrawList()->AddImage((void*)m_framebuffer->m_texID,
			pos, 
			ImVec2(pos.x + m_width - 10, pos.y + m_height - 30)
			, ImVec2(0, 1), ImVec2(1, 0));
	}

	void SceneWindow::PreRender()
	{
		ImVec2 pos = ImGui::GetCursorScreenPos();
		//Render Object to scene camera
		m_framebuffer->BindFrameBuffer();
		//glViewport(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
		GLRenderer::GetInstance()->SetViewPort(0, 0, m_width, m_height);
		//glViewport(0, 0, m_width, m_height); //Set view port to match scene window
		GLRenderer::GetInstance()->SetClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f);
		GLRenderer::GetInstance()->Render(m_sceneCam);
		GLRenderer::GetInstance()->SetDefaultViewport(); //Restore viewport
		m_framebuffer->UnBindFrameBuffer();
	}

	void SceneWindow::Terminate()
	{

	}

	void SceneWindow::SetDrawDebug(bool i)
	{
		GLRenderer::GetInstance()->drawDebug = i;
	}

	void SceneWindow::SceneCameraControl()
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
					zoom_value *= m_sceneCam->GetZoom() * 0.05 * -1.0f;
				else
					zoom_value *= m_sceneCam->GetZoom() * 0.01 * -1.0f;

				m_sceneCam->Zoom(zoom_value);
			}


			if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
			{
				move_value.x += ImGui::GetIO().MouseDelta.x;
				move_value.y -= ImGui::GetIO().MouseDelta.y;
				m_sceneCam->Translate(move_value * -1.0f * m_sceneCam->GetZoom());

			}
		}
	}

}