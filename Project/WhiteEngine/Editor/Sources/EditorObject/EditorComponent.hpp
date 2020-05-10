#pragma once
#include <vector>
#include <string>
#include <map>
#include "EngineCore.hpp"

#define IMPLEMENT_CLONE(TYPE)	inline EditorComponent* clone() const { return new TYPE(); }
#define MAKE_COMPONENT(TYPE)	EditorComponent* TYPE ## _myComponent = EditorComponent::addComponent(#TYPE, new TYPE());
#define INIT_COMPONENT(TYPE)	TYPE* m_typeComponent = nullptr;

namespace Tools
{
	using AvailableComponent = std::vector<std::string>;

	class EditorComponent
	{
	protected:
		static std::map<std::string, EditorComponent*> m_componentTable;
		static AvailableComponent m_availableComponent;
		static inline int m_totalNumEC = 0;
		int m_id = 0;
		bool* m_open = nullptr;
		Component* m_component = nullptr;
		std::string m_componentName;
		bool* m_enable = nullptr;
		virtual void OnRender() {}
		
		virtual void SetChild(bool isChild) {}
		virtual EditorComponent* clone() const = 0;
	public:
		EditorComponent(std::string name, bool canDisable = true, bool canRemove = true);
		~EditorComponent();
		virtual void Init(Component* engineComponent) {}
		bool Render();
		std::string GetName();
		int GetID();
		bool dirty = false;
		Component* GetComponentPtr();

		static EditorComponent* makeComponent(std::string name);
		static EditorComponent* addComponent(std::string type, EditorComponent* comp);
		static AvailableComponent* GetTableList();
	};



	

}