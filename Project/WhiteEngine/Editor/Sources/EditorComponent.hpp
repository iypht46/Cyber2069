#pragma once
#include <vector>
#include <string>
#include <map>
#include "EngineCore.hpp"

#define IMPLEMENT_CLONE(TYPE)	EditorComponent* clone() const { return new TYPE(*this); }
#define MAKE_COMPONENT(TYPE)	EditorComponent* TYPE ## _myComponent = EditorComponent::addComponent(#TYPE, new TYPE());

namespace Tools
{
	using AvailableComponent = std::vector<std::string>;

	class EditorComponent
	{
	protected:
		static std::map<std::string, EditorComponent*> m_componentTable;
		static AvailableComponent m_availableComponent;
		bool m_open = true;
		Component* m_component;
		std::string m_componentName;
		bool* m_enable;
		virtual void OnRender() {}
		
		virtual void SetChild(bool isChild) {}
	public:
		EditorComponent(std::string name) : m_componentName(name) {}
		virtual void Init(Component* engineComponent) {}
		bool Render();
		std::string GetName();
		virtual EditorComponent* clone() const = 0;
		static EditorComponent* makeComponent(std::string name);
		static EditorComponent* addComponent(std::string type, EditorComponent* comp);
		static AvailableComponent* GetTableList();
	};

	



	

}