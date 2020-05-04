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
		TransformEC() : EditorComponent("TransformEC", false) { }
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
		float m_width;
		float m_height;
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
		float m_mass;
		float m_gravityScale;
		float m_drag;
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
		float m_framerate;
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

	class SoundPlayerEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(SoundPlayer)
		bool m_looping;
		float m_volume;
		std::string m_soundName;
		bool m_isSoundLoaded = false;
	protected:
		virtual void OnRender() override;
		IMPLEMENT_CLONE(SoundPlayerEC);
	public:
		SoundPlayerEC() : EditorComponent("SoundPlayerEC") {}
		virtual void Init(Component* soundPlayerComponent) override;
		

	};
	//EDITOR_COMP(RigidbodyEC, Rigidbody)
	//	//TODO: Add pointer to members of each component
	//public:
	//	RigidbodyEC(Rigidbody* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};
}
