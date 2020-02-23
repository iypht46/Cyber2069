#pragma once
#include <string>
#include "EngineCore.hpp"
#define EDITOR_COMP(NAME, TYPE)	\
class NAME : EditorComponent	\
{	private: TYPE* m_component;	\

namespace Tools
{
	class EditorComponent
	{
	protected:
		bool m_open = true;
		Component* m_component;
		std::string m_componentName;
		virtual void OnRender() {}
		virtual void Init() {}
		virtual void SetChild(bool isChild) {}
	public:
		EditorComponent(std::string name, Component* comp) 
			: m_componentName(name) , m_component(comp) {}
		void Render();
		
	};

	class TransformEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		glm::vec3* m_position;
		glm::vec3* m_scale;
		float* m_rotation;
	protected:
		virtual void Init();
		virtual void OnRender() override;
		virtual void SetChild(bool isChild) override;
	public:
		TransformEC(Transform* component);
		
	};

	class MeshRendererEC : public EditorComponent
	{
	private:
		//TODO: Add pointer to members of each component
		unsigned int* m_tex;
		
	protected:
		virtual void OnRender() override;
	public:
		MeshRendererEC(MeshRenderer* component);
		virtual void Init() override;
		
	};

	//EDITOR_COMP(ColliderEC, Collider)
	//	//TODO: Add pointer to members of each component
	//public:
	//	ColliderEC(Collider* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};

	//EDITOR_COMP(RigidbodyEC, Rigidbody)
	//	//TODO: Add pointer to members of each component
	//public:
	//	RigidbodyEC(Rigidbody* component) : m_component(component) { this->Init(); }
	//	virtual void Init() override;
	//	virtual void Render() override;
	//};

	

}