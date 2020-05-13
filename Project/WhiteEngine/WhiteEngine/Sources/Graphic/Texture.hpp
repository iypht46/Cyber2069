#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "GL/glew.h"

namespace Graphic
{
	struct Texture
	{
		GLuint m_textureID = -1;
		glm::vec2 m_size;

		Texture()
		{
			m_size = glm::vec2(1, 1);
		}

		Texture(GLuint id, int w, int h) : m_textureID(id)
		{
			m_size = glm::vec2(w, h);
		}
	};
}