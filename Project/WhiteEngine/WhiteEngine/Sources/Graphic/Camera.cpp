#include "Camera.hpp"
#include "GLRenderer.h"
#include "Window.hpp"

namespace Graphic
{
	CameraObject* Camera;

	CameraObject::CameraObject()
	{
		ResetCam();
	}

	CameraObject* getCamera()
	{
		if (Camera == nullptr)
		{
			Camera = new CameraObject();
		}
		return Camera;
	}

	void CameraObject::Translate(glm::vec3 translate)
	{
		m_position.x += translate.x;
		m_position.y += translate.y;
		SetPos(m_position);
	}

	void CameraObject::Rotate(float rotation)
	{

	}

	void CameraObject::Zoom(float zoom)
	{
		m_zoom += zoom;
		SetZoom(m_zoom);
	}

	void CameraObject::SetPos(glm::vec3 pos)
	{
		m_position.x = pos.x;
		m_position.y = pos.y;
		m_view = glm::lookAt(m_position, m_position + m_dir, m_dirUp);
	}

	void CameraObject::SetRotate(glm::vec3 rotation)
	{

	}

	void CameraObject::SetZoom(float zoom)
	{
		m_zoom = zoom;
		if (m_zoom < 0.1f)
		{
			m_zoom = 0.1f;
		}

		m_projection = glm::ortho(-(Window::GetWidth() / 2) * m_zoom, (Window::GetWidth() / 2) * m_zoom, -(Window::GetHeight() / 2)* m_zoom, (Window::GetHeight() / 2)* m_zoom);
		m_view = glm::lookAt(m_position, m_position + m_dir, m_dirUp);
	}

	void CameraObject::ResetCam()
	{
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

		m_zoom = 1.0f;

		m_dir = glm::vec3(0.0f, 0.0f, -1.0f);
		m_dirUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_projection = GLRenderer::GetInstance()->GetprojectionMatrix();
		m_view = glm::lookAt(m_position, m_position + m_dir, m_dirUp);
	}

	glm::mat4& CameraObject::GetProjectionMatrix()
	{
		return m_projection;
	}

	glm::mat4& CameraObject::GetViewMatrix()
	{
		return m_view;
	}

	glm::vec3 CameraObject::GetCampos() {
		return m_position;
	}

	float CameraObject::GetZoom() {
		return m_zoom;
	}

	/*glm::mat4 CameraObject::CalculateViewMatrix(glm::vec3 position, glm::vec3 dir)
	{

	}*/

}
