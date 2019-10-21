#include "Core/EC/Components/Rigidbody.hpp"

namespace Core
{
	void Rigidbody::Initialize(Collider& c)
	{
		m_collider = &c;
	}

	Rigidbody::~Rigidbody()
	{

	}

	void Rigidbody::SetKinematic(bool i)
	{
		m_isKinematic = i;
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

