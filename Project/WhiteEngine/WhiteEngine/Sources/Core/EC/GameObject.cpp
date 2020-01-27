#include "GameObject.hpp"
#include "../Factory.h"
#include "Components/BehaviourScript.h"


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