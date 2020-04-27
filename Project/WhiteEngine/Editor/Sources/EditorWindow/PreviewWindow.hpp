#pragma once
//Editor
#include "EditorWindow/UIWindow.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "EditorObject/CoreComponentEC.hpp"
//WhiteEngine
#include "Graphic/Framebuffer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/Camera.hpp"


namespace Tools
{
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
		//Test
		unsigned int VBO, VAO;
		int shaderProgram;
	protected:
		virtual void Init() override;
		virtual void Terminate() override;
		virtual void PreRender() override;
		virtual void OnRender() override;
		virtual void PostRender() override;
		
		//void DrawGrid(glm::vec3 color);
	public:
		PreviewWindow(bool* open) : UIWindow("Prefab Preview", 600, 600, open, false) { this->Init(); }
		~PreviewWindow() {}
		bool SetMesh(EditorComponent* mesh);
		void SetEntity(EditorEntity* ent);
		
	};
}


