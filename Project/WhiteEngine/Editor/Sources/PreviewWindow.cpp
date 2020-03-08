#include "PreviewWindow.hpp"

namespace Tools
{

	void PreviewWindow::Init()
	{
		m_framebuffer.Init(m_width, m_height);
		m_shader = new Graphic::Shader("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		m_textureLoaded = false;
		m_camera = new Graphic::CameraObject();
		m_windowFlags |= ImGuiWindowFlags_NoResize;
		m_bgColor = glm::vec4(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f, 1.0f);
		//m_windowFlags |= ImGuiWindowFlags_NoCollapse;
		//m_windowFlags &= ImGuiWindowFlags_NoMove;
		// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};

		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}

	void PreviewWindow::OnRender()
	{
		// Get the current cursor position (where your window is)
		ImVec2 pos = ImGui::GetCursorScreenPos();
		/*ImGui::GetWindowDrawList()->AddImage((void *)m_framebuffer.m_texID,
			ImVec2(ImGui::GetItemRectMin().x - pos.x, ImGui::GetItemRectMin().y - pos.y)
			, ImVec2(pos.x + m_width - 10, pos.y + m_height - 30), ImVec2(0, 1), ImVec2(1, 0));*/

		ImGui::GetWindowDrawList()->AddImage((void *)m_framebuffer.m_texID,
			pos, ImVec2(pos.x + m_width - 10, pos.y + m_height - 30), ImVec2(0, 1), ImVec2(1, 0));
		
			
	}

	void PreviewWindow::PreRender()
	{
		m_framebuffer.BindFrameBuffer();
		glViewport(0, 0, m_height, m_height);
		glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (m_mesh && m_mesh->IsTextureLoaded())
		{
			m_shader->use();
			m_mesh->RenderTexture(glm::mat4(1.0f));
			m_shader->unUse();
		}
		/*if (m_mesh)
		{
			m_shader->use();
			m_mesh->RenderTexture(glm::mat4(1.0f));
			m_shader->unUse();
		}*/
		//if (!m_textureLoaded)
		//{
		//	//render object in to framebuffer
		//	//m_shader->use();
		//	glUseProgram(shaderProgram);
		//	glBindVertexArray(VAO); // seeing as we only have a single vao there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//	glDrawArrays(GL_TRIANGLES, 0, 3); 
		//	//m_shader->unuse();
		//	
		//}

		m_framebuffer.UnBindFrameBuffer();
	}

	void PreviewWindow::DrawGrid(glm::vec3 color)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(color.x, color.y, color.z);
		int xsize = 0, ysize = 0;
		for (int j = 0; j < 10; j++)
		{

			xsize = 0;
			for (int i = 0; i < 10; i++)
			{
				glBegin(GL_POLYGON);
				glVertex3f(-50.0 + xsize, -50.0 + ysize, 0.0);
				glVertex3f(-40.0 + xsize, -50.0 + ysize, 0.0);
				glVertex3f(-40.0 + xsize, -40.0 + ysize, 0.0);
				glVertex3f(-50.0 + xsize, -40.0 + ysize, 0.0);
				glEnd();
				xsize += 10.0;
			}
			ysize += 10.0;
		}
		glFlush();
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

}


