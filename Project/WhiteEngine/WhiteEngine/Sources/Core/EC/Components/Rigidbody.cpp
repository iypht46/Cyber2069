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

}

