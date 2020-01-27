#include "BehaviourScript.h"
#include "../../Factory.h"
#include "Core/EC/GameObject.hpp"
#include <vector>
BehaviourScript::BehaviourScript() {
	Factory<BehaviourScript>::Add(this);
}

void BehaviourScript::SetGameObject(GameObject * obj)
{
	m_gameObject = obj;
	m_gameObject->m_scripts.push_back(this);
}
