#include "Core/EC/Components/BehaviourScript.h"
#include "Core/Factory.h"
#include "Core/EC/GameObject.hpp"
#include "Physic/Collision.hpp"

BehaviourScript::BehaviourScript() {
	Factory<Component, BehaviourScript>::Add(this);
}

BehaviourScript::~BehaviourScript() {
	Factory<Component, BehaviourScript>::Remove(this);
}

void BehaviourScript::Init() { }

void BehaviourScript::OnAwake() { /*ENGINE_INFO("raw behav init");*/ }
void BehaviourScript::OnEnable() { }
void BehaviourScript::OnDisable() { }

void BehaviourScript::OnStart() { }
void BehaviourScript::OnUpdate(float dt) { }
void BehaviourScript::OnFixedUpdate(float dt) { }

void BehaviourScript::OnTriggerEnter(const Physic::Collision col) { }
void BehaviourScript::OnTriggerStay(const Physic::Collision col) { }
void BehaviourScript::OnTriggerExit(const Physic::Collision col) { }

void BehaviourScript::OnCollisionEnter(const Physic::Collision col) { }
void BehaviourScript::OnCollisionStay(const Physic::Collision col) { }
void BehaviourScript::OnCollisionExit(const Physic::Collision col) { }
