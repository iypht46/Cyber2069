#pragma once
#include "EditorComponent.hpp"
#include "imgui.h"
#include "imfilebrowser.h"
#include "Utility/Filesystem.hpp"

namespace Tools
{
	class TransformEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(Transform)
		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
		float m_rotation = 0.0f;
	protected:
		virtual void OnRender() override;
		IMPLEMENT_CLONE(TransformEC);
	public:
		TransformEC() : EditorComponent("TransformEC", false, false) { }
		virtual void SetParent(TransformEC* other);
		virtual void Init(Component* engineComponent) override;
		
	};

	class MeshRendererEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(MeshRenderer)
		//TODO: Add pointer to members of each component
		bool m_isLoaded = false;
		bool m_isUI = false;
		bool m_isReplaceColor = false;
		glm::vec3 m_replaceColor = glm::vec3(1.0f, 1.0f, 1.0f);
		std::string m_textureName;
		Utility::fs::path m_texturePath;
		ImGui::FileBrowser m_textureFileDialog;
	protected:
		virtual void OnRender() override;
		virtual bool AddTexture(std::string path);
		IMPLEMENT_CLONE(MeshRendererEC);
	public:
		MeshRendererEC() : EditorComponent("MeshRendererEC") { }
		virtual void Init(Component* engineComponent) override;
		void RenderTexture(glm::mat4 matrix);
		bool IsTextureLoaded();
		
	};

	class BoxColliderEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		INIT_COMPONENT(BoxCollider)
		float m_width = 0.0f;
		float m_height = 0.0f;
	protected:
		virtual void OnRender() override;
		IMPLEMENT_CLONE(BoxColliderEC);
	public:
		BoxColliderEC() : EditorComponent("BoxColliderEC") {}
		virtual void Init(Component* engineComponent) override;
		
	};

	class RigidbodyEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(Rigidbody)
		float m_mass = 0.0f;
		float m_gravityScale = 1.0f;
		float m_drag = 0.0f;
	protected:
		virtual void OnRender() override;
		IMPLEMENT_CLONE(RigidbodyEC);
	public:
		RigidbodyEC() : EditorComponent("RigidbodyEC") {}
		virtual void Init(Component* engineComponent) override;
		
	};

	class AnimatorEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(Animator)
		float m_framerate = 12;
		Utility::fs::path m_controllerPath;
		std::shared_ptr<AnimationController> m_controller;
		ImGui::FileBrowser m_acFileDialog;
	protected:
		virtual void OnRender() override;
		IMPLEMENT_CLONE(AnimatorEC);
	public:
		AnimatorEC() : EditorComponent("AnimatorEC") {}
		virtual void Init(Component* animatorComponent) override;
		void SetController(std::shared_ptr<AnimationController> controller);
		void SetCurrentState(int index);

	};
}
