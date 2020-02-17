#pragma once
#include <GL\glew.h>
#include "Graphic/Shader.hpp"

namespace Graphic
{
	struct Framebuffer
	{
		int m_frameW = 800, m_frameH = 800;
		GLuint m_fboID;
		GLuint m_texID;
		GLuint m_screenVAO;
		GLuint m_screenVBO;
		Shader* m_screenShader;


		//Interface
		Framebuffer();
		Framebuffer(int w, int h);
		bool Init(int w, int h);
		void Render();
		void Terminate();
		void BindFrameBuffer();
		void UnBindFrameBuffer();

	};
}

