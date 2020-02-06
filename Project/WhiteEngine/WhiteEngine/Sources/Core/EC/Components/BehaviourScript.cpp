#include "Core/EC/Components/BehaviourScript.h"
#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "Physic/Collision.hpp"

BehaviourScript::BehaviourScript() {
	Factory<BehaviourScript>::Add(this);
}

void BehaviourScript::SetGameObject(GameObject * obj)
{
	m_gameObject = obj;
	m_gameObject->m_scripts.push_back(this);
}

void BehaviourScript::OnAwake() { }
void BehaviourScript::OnEnable() { }
void BehaviourScript::OnDisable() { }

void BehaviourScript::OnTriggerEnter(const Physic::Collision col) { }
void BehaviourScript::OnTriggerStay(const Physic::Collision col) { }
void BehaviourScript::OnTriggerExit(const Physic::Collision col) { }

void BehaviourScript::OnCollisionEnter(const Physic::Collision col) { }
void BehaviourScript::OnCollisionStay(const Physic::Collision col) { }
void BehaviourScript::OnCollisionExit(const Physic::Collision col) { }
