#pragma once

#include "Graphic/Framebuffer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/Camera.hpp"
#include "UIWindow.hpp"
#include "CoreComponentEC.hpp"

namespace Tools
{
	class PreviewWindow : public UIWindow
	{
	private:
		Graphic::Framebuffer m_framebuffer;
		Graphic::Shader* m_shader;
		Graphic::CameraObject* m_camera;
		MeshRendererEC* m_mesh;
		bool m_textureLoaded;
		glm::vec4 m_bgColor;
		//Test
		unsigned int VBO, VAO;
		int shaderProgram;
	protected:
		virtual void Init() override;
		virtual void Terminate() override;
		virtual void OnRender() override;
		
		void DrawGrid(glm::vec3 color);
	public:
		PreviewWindow(bool* open) : UIWindow("Preview", 500, 500, open) { this->Init(); }
		~PreviewWindow() {}
		bool SetMesh(EditorComponent* mesh);
		virtual void PreRender() override;
	};
}


