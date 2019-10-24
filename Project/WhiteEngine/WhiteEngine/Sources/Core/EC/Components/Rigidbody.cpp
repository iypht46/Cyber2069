#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"

namespace Core
{
	void Rigidbody::Initialize(Collider& c)
	{
		m_collider = &c;
	}

	//Create BoxCollider
	void Rigidbody::Initialize(float hW, float hH)
	{
		//Create new box collider
		BoxCollider* col = m_gameObject->AddComponent<BoxCollider>();
		//Init Box
		col->SetBox(hW, hH, this);
		//Set Collider
		m_collider = col;
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

}

