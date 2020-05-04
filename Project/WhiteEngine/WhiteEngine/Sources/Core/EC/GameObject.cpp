#include "GameObject.hpp"
#include "Core/Factory.h"
#include "Core/EC/Components/BehaviourScript.h"
#include "Physic/Collision.hpp"

#include "Core/Logger.hpp"

int GameObject::s_IDCounter = 0;

GameObject::GameObject()
{
	m_objectID = GameObject::s_IDCounter++;
	m_transform = make_shared<Transform>();

	Factory<void, GameObject>::Add(this);
}

void GameObject::SetActive(bool activestate) {

	//call behaviour
	for (std::shared_ptr<BehaviourScript> behaviour : m_scripts) {

		if (activestate) {
			behaviour->OnEnable();
		}
		else if (!activestate) {
			behaviour->OnDisable();

		}
	}

	isActive = activestate;
}

//return object active state
bool GameObject::Active() {
	return isActive;
}

int GameObject::GetID() {
	return m_objectID;
}

void GameObject::InitComponents() {

	for (std::shared_ptr<Component> component : m_components) {
		component->SetGameObject(this);
		component->Init();

		//if is behaviou script, also assign to script collection
		std::shared_ptr<BehaviourScript> behaviour = dynamic_pointer_cast<BehaviourScript>(component);
		if (behaviour) {
			m_scripts.push_back(behaviour);
		}
	}

	for (std::shared_ptr<BehaviourScript> behaviour : m_scripts) {
		behaviour->OnAwake();
	}
}

void GameObject::StartComponents() {

	for (std::shared_ptr<BehaviourScript> behaviour : m_scripts) {
		behaviour->OnStart();
	}
}

void GameObject::CollisionEnter(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnCollisionEnter(col);
	}
}

void GameObject::CollisionStay(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnCollisionStay(col);
	}
}

void GameObject::CollisionExit(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnCollisionExit(col);
	}
}

void GameObject::TriggerEnter(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnTriggerEnter(col);
	}
}

void GameObject::TriggerStay(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnTriggerStay(col);
	}
}

void GameObject::TriggerExit(const Physic::Collision col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnTriggerExit(col);
	}
}
