#include "GameObject.hpp"
#include "Core/Factory.h"
#include "Core/EC/Components/BehaviourScript.h"
#include "Physic/Collision.hpp"

void GameObject::SetActive(bool activestate) {
	isActive = activestate;
}

//return object active state
bool GameObject::Active() {
	return isActive;
}

void GameObject::OnCollisionEnter(const Physic::Collision* col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
		scripts->OnCollisionEnter(col);
	}
}

void GameObject::OnCollisionExit(const Physic::Collision* col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
	}
}

void GameObject::OnTriggerEnter(const Physic::Collision* col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
	}
}

void GameObject::OnTriggerExit(const Physic::Collision* col)
{
	for (auto scripts : m_scripts)
	{
		//scripts
	}
}

GameObject::GameObject() 
{
	isActive = true;
}