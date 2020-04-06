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
		//TODO: Add pointer to members of each component
		INIT_COMPONENT(Transform)

		glm::vec3 m_position = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
		float m_rotation = 0.0f;
	protected:

		virtual void OnRender() override;
		virtual void SetChild(bool isChild) override;
	public:
		TransformEC() : EditorComponent("TransformEC") { }
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(TransformEC);
	};

	class MeshRendererEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(MeshRenderer)
		//TODO: Add pointer to members of each component
		bool m_isLoaded = false;
		std::string m_textureName;
		Utility::fs::path m_texturePath;
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
		INIT_COMPONENT(BoxCollider)
		glm::vec3 m_colliderScale;
		float m_width;
		float m_height;
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
		INIT_COMPONENT(Rigidbody)
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

	class AnimatorEC : public EditorComponent
	{
	private:
		INIT_COMPONENT(Animator)
		AnimationController* m_animationController;
		float* m_framerate;
	protected:
		virtual void OnRender() override;
	public:
		AnimatorEC() : EditorComponent("AnimatorEC") {}
		virtual void Init(Component* animatorComponent) override;
		void RenderAnimation(Animation* anim);
		void RenderAnimationController();
		IMPLEMENT_CLONE(AnimatorEC);

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
	public:
		SoundPlayerEC() : EditorComponent("SoundPlayerEC") {}
		virtual void Init(Component* soundPlayerComponent) override;
		IMPLEMENT_CLONE(SoundPlayerEC);

	};
	//EDITOR_COMP(RigidbodyEC, Rigidbody)
	//	//TODO: Add pointer to members of each component
	//public:
	//	RigidbodyEC(Rigidbody* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};
}
