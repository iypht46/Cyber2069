#pragma once
#include "Core/EC/Components/Component.hpp"
namespace Tools
{
	class EditorComponent
	{
	private:
		Component* m_component;
	public:
		EditorComponent(Component* comp) : m_component(comp) {}

	};
}