#include "Graphic/GraphicCore.hpp"
#include "Camera.hpp"
#include "Core/Logger.hpp"


namespace Graphic
{
	void Init(void)
	{
		//ENGINE_INFO()
		Window::Init("White Engine", Window::WindowMode::WINDOWED);

		////////////OpenGL////////////
		g_renderer = GLRenderer::GetInstance();

		g_renderer->InitGL("Sources/Shader/vertext.shd", "Sources/Shader/fragment.shd");
		g_renderer->SetOrthoProjection(-Window::GetWidth() / 2, Window::GetWidth() / 2, -Window::GetHeight() / 2, Window::GetHeight() / 2);
		g_renderer->SetClearColor(72.0f/255.0f, 42.0f / 255.0f, 109.0f / 255.0f);
		getCamera()->ResetCam();
		//getCamera()->ResetCam();

		ENGINE_WARN("Graphic System Initialized");

		//g_renderer->test = new MeshRenderer("Sources/Mockup_PlayerBody_Vversion02.png", 7, 5);

	}

	void Render(void)
	{
		if (!Window::ShouldClose())
		{
			glm::mat4 globalMatrix = getCamera()->GetProjectionMatrix() * getCamera()->GetViewMatrix();
			g_renderer->Render(globalMatrix);
			Window::SwapBuffer();
			glfwPollEvents();
		}
	}

	void Terminate(void)
	{
		//Terminate Window
		Window::Terminate();
		//Terminate Glfw
		glfwTerminate();

		ENGINE_WARN("Graphic System Terminated");
	}

	void EnableFrameBuffer(FBO_STATE state)
	{
		g_renderer->EnableFBO(state, Window::GetWidth(), Window::GetHeight());
	}

	CameraObject* getCamera()
	{
		if (g_camera == nullptr)
		{
			g_camera = new CameraObject();
		}
		return g_camera;
	}
}
