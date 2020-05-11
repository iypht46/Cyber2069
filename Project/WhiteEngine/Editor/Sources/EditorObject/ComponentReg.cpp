//Editor
#include "EditorObject/CoreComponentEC.hpp"
#include "EditorObject/ParticleComponentEC.hpp"
#include "EditorObject/ScriptEC.hpp"

namespace Tools
{
	//*****Prototype Design Variable*****//
	std::map<std::string, EditorComponent*> EditorComponent::m_componentTable;
	AvailableComponent EditorComponent::m_availableComponent;
	MAKE_COMPONENT(TransformEC);
	MAKE_COMPONENT(MeshRendererEC);
	MAKE_COMPONENT(BoxColliderEC);
	MAKE_COMPONENT(RigidbodyEC);
	MAKE_COMPONENT(AnimatorEC);
	MAKE_COMPONENT(ParticleComponentEC);
	//*****          END            *****//
}