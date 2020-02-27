#pragma once
#include "EditorComponent.hpp"

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
		unsigned int* m_tex;
		std::string m_textureName;
	protected:
		virtual void OnRender() override;
		virtual bool AddTexture(std::string path);
	public:
		MeshRendererEC() : EditorComponent("MeshRenderer") { }
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(MeshRendererEC);
	};

	class BoxColliderEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
	protected:
		virtual void OnRender() override;
		glm::vec3* m_colliderScale;
		float* m_width;
		float* m_height;
	public:
		BoxColliderEC() : EditorComponent("BoxColliderEC") {}
		virtual void Init(Component* engineComponent) override;
		IMPLEMENT_CLONE(BoxColliderEC);
	};

	//EDITOR_COMP(RigidbodyEC, Rigidbody)
	//	//TODO: Add pointer to members of each component
	//public:
	//	RigidbodyEC(Rigidbody* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};
}
