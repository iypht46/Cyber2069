#pragma once
#include <GL\glew.h>
#include "Graphic/Shader.hpp"

namespace Graphic
{
	class Framebuffer
	{
	private:
		int m_frameW = 800, m_frameH = 800;
		
		unsigned int m_screenVAO;
		unsigned int m_screenVBO;
	protected:
		void GenFrameBuffer();
		void GenTextureBuffer(int w, int h);
	public:
		unsigned int m_texID;
		unsigned int m_fboID;
		Shader* m_screenShader;
		//Interface
		Framebuffer();
		Framebuffer(int w, int h);
		bool Init(int w, int h);
		void Render();
		void Terminate();
		//Interface
		void BindFrameBuffer();
		void UnBindFrameBuffer();
		

	};
}

