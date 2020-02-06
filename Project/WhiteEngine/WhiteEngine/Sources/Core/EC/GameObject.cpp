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

GameObject::GameObject() 
{
	isActive = true;
}