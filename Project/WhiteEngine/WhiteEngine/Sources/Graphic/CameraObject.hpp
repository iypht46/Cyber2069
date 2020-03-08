#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.hpp"

namespace Graphic
{
	struct CameraObject
	{
		//Camera Info
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		float m_zoom;

		//Camera Vector
		glm::vec3 m_dir;
		glm::vec3 m_dirRight;
		glm::vec3 m_dirUp;
		glm::vec3 m_dirFront;

		//Matrix
		glm::mat4 m_view;
		glm::mat4 m_projection;

		CameraObject();

		//Camera Interface
		void Translate(glm::vec3 translate);
		void Rotate(float rotation);
		void Zoom(float zoom);


		void SetPos(glm::vec3 pos);
		void SetRotate(glm::vec3 rotation);
		void SetZoom(float zoom);

		void ResetCam();

		glm::mat4& GetProjectionMatrix(void);
		glm::mat4& GetViewMatrix(void);

		glm::vec3 GetCampos();
		float GetZoom();

		//Static Method
		//static glm::mat4 CalculateViewMatrix(glm::vec3 position, glm::vec3 dir);
		//static glm::mat4 CalculateProjectionMatrix(float size, float aspect, )

	};
}