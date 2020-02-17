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
		//Load Shader
		m_screenShader = new Shader("Sources/Shader/fbo_ver.shd", "Sources/Shader/fbo_frag.shd");
		m_screenShader->use();
		m_screenShader->setInt("screenTexture", 0);
		
	//	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
	//    // positions   // texCoords
	//    -0.3f,  1.0f,  0.0f, 1.0f,
	//    -0.3f,  0.7f,  0.0f, 0.0f,
	//     0.3f,  0.7f,  1.0f, 0.0f,

	//    -0.3f,  1.0f,  0.0f, 1.0f,
	//     0.3f,  0.7f,  1.0f, 0.0f,
	//     0.3f,  1.0f,  1.0f, 1.0f
	//};
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

		glGenBuffers(1, &m_fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID); 

		// create a color attachment texture
		glGenTextures(1, &m_texID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_frameW, m_frameH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texID, 0);

		//Check if framebuffer have been initialized
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			ENGINE_INFO("Generate Framebuffer Complete");
			return true;
		}
		else
		{
			ENGINE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void Framebuffer::Render()
	{
		m_screenShader->use();
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(m_screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texID);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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
}

