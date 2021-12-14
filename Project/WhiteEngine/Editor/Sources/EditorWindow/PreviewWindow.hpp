#pragma once
//Editor
#include "EditorWindow/UIWindow.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "EditorObject/CoreComponentEC.hpp"
//WhiteEngine
#include "Graphic/Framebuffer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/Camera.hpp"
//Standard Header
#include <functional>

namespace Tools
{
	using RenderFunction = std::function<void(glm::mat4)>;

	class PreviewWindow : public UIWindow
	{
	private:
		Graphic::Framebuffer m_framebuffer;
		Graphic::Shader* m_shader;
		Graphic::CameraObject* m_camera;
		EditorEntity* m_entityToRender;
		MeshRendererEC* m_mesh;
		bool m_textureLoaded;
		glm::vec4 m_bgColor;
		RenderFunction m_customRenderFunction;
		bool m_customRender = false;
		//Test
		unsigned int VBO, VAO;
		int shaderProgram;
	protected:
		virtual void Init() override;
		virtual void Terminate() override;
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void PostRender() override;
		
		void PreviewCameraControl();
		
		//void DrawGrid(glm::vec3 color);
	public:
		PreviewWindow(bool* open, Container::wString name, bool hideToggle = false, int width = 600, int height = 600) : UIWindow(name, width, height, open, hideToggle) { this->Init(); }
		~PreviewWindow() { this->Terminate(); }
		bool SetMesh(EditorComponent* mesh);
		void SetEntity(EditorEntity* ent);
		void SetCustomRenderFunction(RenderFunction func);
		void SetCustomRender(bool i);
	};
}


