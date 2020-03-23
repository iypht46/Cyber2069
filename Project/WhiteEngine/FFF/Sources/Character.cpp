#include "Character.hpp"

void Character::flip() 
{
	m_gameObject->m_transform.SetScale(glm::vec3(m_gameObject->m_transform.GetScale().x * -1.0f, m_gameObject->m_transform.GetScale().y, m_gameObject->m_transform.GetScale().z));
	facingRight = !facingRight;
}