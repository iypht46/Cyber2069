#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"


Rigidbody::Rigidbody()
{
	m_position = glm::vec3(0, 0, 0);
	m_orientation = 0.0f;
}

//Create BoxCollider
void Rigidbody::Init(float hW, float hH)
{
	//Create new box collider
	BoxCollider* col = m_gameObject->AddComponent<BoxCollider>();
	//Init Box
	col->Init(hW, hH, this);
	//Set Collider
	m_collider = col;
	m_transform = &m_gameObject->m_transform;
}

Rigidbody::~Rigidbody()
{
	
}

void Rigidbody::OnAwake()
{	 
}	 
	 
void Rigidbody::OnEnable()
{	 
}	 
	 
void Rigidbody::OnStart()
{	 
}	 
	 
void Rigidbody::OnDisable()
{	 
}	 
	 
void Rigidbody::OnUpdate(float dt)
{	
}	
	
void Rigidbody::OnFixedUpdate(float dt)
{	 
}	 
	 
void Rigidbody::OnDestroy()
{
}

