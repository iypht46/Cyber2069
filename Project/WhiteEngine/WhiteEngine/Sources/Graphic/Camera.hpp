#pragma once
#include <glm/glm.hpp>

namespace Graphic
{
	struct CameraObject
	{
		//Camera Info
		glm::vec3 m_position;
		glm::vec3 m_rotation;

		//Camera Vector
		glm::vec3 m_dirRight;
		glm::vec3 m_dirUp;
		glm::vec3 m_dirFront;

		//Matrix
		glm::mat4 m_view;
		glm::mat4 m_projection;

		CameraObject();

		//Camera Interface
		void Move(glm::vec3);
		void Translate(glm::vec3 pos);
		void Rotate(float);

		glm::mat4& GetProjectionMatrix(void);
		glm::mat4& GetViewMatrix(void);

		//Static Method
		static glm::mat4 CalculateViewMatrix(glm::vec3 position, glm::vec3 dir);
		//static glm::mat4 CalculateProjectionMatrix(float size, float aspect, )

	};
}