#pragma once
#include "Core/EC/Components/Component.hpp"
namespace Tools
{
	class EditorComponent
	{
	protected:
		Component* m_component;
	public:
		EditorComponent(Component* comp) : m_component(comp) {}
		virtual void Render() = 0;

	};

	class MeshRendererEC : EditorComponent
	{

	};

	class TransformEC : EditorComponent
	{

	};

}