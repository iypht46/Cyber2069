#include "Framebuffer.hpp"

namespace Graphic
{
	Framebuffer::Framebuffer()
	{
		
	}

	Framebuffer::Framebuffer(int w, int h)
	{
		m_frameW = w;
		m_frameH = h;
	}

	bool Framebuffer::Init(int w, int h)
	{

		m_frameW = w;
		m_frameH = h;
		//std::cout << "Frame W/H: " << m_frameW << " / " << m_frameH << std::endl;
		//Load Shader
		m_screenShader = new Shader("Sources/Shader/fbo_ver.shd", "Sources/Shader/fbo_frag.shd");
		
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		//Create screen quad vertex array
		glGenVertexArrays(1, &m_screenVAO);
		glGenBuffers(1, &m_screenVBO);
		glBindVertexArray(m_screenVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		m_screenShader->use();
		m_screenShader->setInt("screenTexture", 0);
		//Generate framebuffer and assign it to m_fboID of this class
		GenFrameBuffer();
		//Bind generated framebuffer
		BindFrameBuffer();
		GenTextureBuffer(m_frameW, m_frameH);
		//Bind Texture with frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);

		//Check if framebuffer have been initialized properly
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//Unbind framebuffer
			UnBindFrameBuffer();
			return false;
		}

		//Unbind framebuffer
		UnBindFrameBuffer();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		return true;
	}

	void Framebuffer::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT); //Clear color buffer
		m_screenShader->use(); //Use Screen Shader
		glBindVertexArray(m_screenVAO);
		glBindTexture(GL_TEXTURE_2D, m_texID);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6); //Draw bound array buffer
		//Unbind program
		glUseProgram(NULL);
	}

	void Framebuffer::Terminate()
	{
		glDeleteVertexArrays(1, &m_screenVAO);
		glDeleteBuffers(1, &m_screenVBO);
		glDeleteFramebuffers(1, &m_fboID);
	}

	void Framebuffer::BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		
	}

	void Framebuffer::UnBindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::GenFrameBuffer()
	{
		glGenFramebuffers(1, &m_fboID);
	}

	void Framebuffer::GenTextureBuffer(int w, int h)
	{
		// create a color attachment texture
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

}

