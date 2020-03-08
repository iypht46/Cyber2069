#pragma once
#include "EditorComponent.hpp"
#include "imgui.h"
#include "imfilebrowser.h"

namespace Tools
{
	class TransformEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		glm::vec3* m_position;
		glm::vec3* m_scale;
		float* m_rotation;
	protected:

		virtual void OnRender() override;
		virtual void SetChild(bool isChild) override;
	public:
		TransformEC() : EditorComponent("Transform") { }
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(TransformEC);
	};

	class MeshRendererEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		bool m_isLoaded = false;
		std::string m_textureName;
		ImGui::FileBrowser m_textureFileDialog;
	protected:
		virtual void OnRender() override;
		virtual bool AddTexture(std::string path);
	public:
		MeshRendererEC() : EditorComponent("MeshRendererEC") { }
		virtual void Init(Component* engineComponent) override;
		void RenderTexture(glm::mat4 matrix);
		bool IsTextureLoaded();
		IMPLEMENT_CLONE(MeshRendererEC);
	};

	class BoxColliderEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		glm::vec3* m_colliderScale;
		float* m_width;
		float* m_height;
	protected:
		virtual void OnRender() override;
	public:
		BoxColliderEC() : EditorComponent("BoxColliderEC") {}
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(BoxColliderEC);
	};

	class RigidbodyEC : public EditorComponent
	{
	private:
		glm::vec3* m_velocity;
		glm::vec3* m_acceleration;
		float* m_mass;
		float* m_gravityScale;
		float* m_drag;
	protected:
		virtual void OnRender() override;
	public:
		RigidbodyEC() : EditorComponent("RigidbodyEC") {}
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(RigidbodyEC);
	};
	//EDITOR_COMP(RigidbodyEC, Rigidbody)
	//	//TODO: Add pointer to members of each component
	//public:
	//	RigidbodyEC(Rigidbody* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};
}
