#pragma once
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"


#define BOX_VERTICES 4

	enum class ColliderType : int
	{
		INVALID = -1,
		BOX,
		CIRCLE
	};

	class Rigidbody;
	class Transform;

	class Collider : public Component
	{
	protected:
		Transform* m_transform;
		Rigidbody* m_rigidbody;

		ColliderType m_colliderType;
		bool m_isStatic = true;
		bool m_isTrigger;
		float m_density;
	public:
		
		//Constructor
		Collider(ColliderType col) 
			: m_colliderType(col), m_density(1.0f) {}
		//Destructor
		~Collider() {};
		//Collider Interface
		void SetTrigger(bool);
		ColliderType GetType();
		bool IsStatic();

		//Virtual
		virtual void SetOrient() {};
		virtual void Update(float) const {}
		virtual void ComputeMass() {};

	};

	class BoxCollider : public Collider
	{
		friend class Rigidbody;
	private:
		float m_width;
		float m_height;
	public:
		//Constructor
		BoxCollider() : Collider(ColliderType::BOX) {}
		void Init(float, float);
		void Init(float, float, Rigidbody*);

		//Box Collider Interface
		virtual void ComputeMass();
		//virtual void Update(float) const;
	};


