#pragma once
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"


#define BOX_VERTICES 4

	enum class ColliderType : int
	{
		INVALID = -1,
		BOX,
		COLNUM
	};

	class Rigidbody;
	class Transform;
	namespace Physic
	{
		struct AABB;
	}

	class Collider : public Component
	{
	protected:
		
		

		ColliderType m_colliderType;
		bool m_isStatic = true;
		bool m_isTrigger;
		float m_density;
	public:
		Transform* m_transform;
		Rigidbody* m_rigidbody;

		//Constructor
		Collider(ColliderType col) 
			: m_colliderType(col), m_density(1.0f) {}
		//Destructor
		~Collider() {};
		//Collider Interface
		void SetTrigger(bool);
		bool IsTrigger(void);
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
		float m_halfWidth;
		float m_halfHeight;
		glm::vec3 m_colliderScale; //Scale multiplier in halfwidth and halfheight
	public:
		//Constructor
		BoxCollider() : Collider(ColliderType::BOX) {}
		void Init(float, float);
		void Init(float, float, Rigidbody*);
		void ComputeAABB(Physic::AABB&);

		//Box Collider Interface
		virtual void ComputeMass();
		virtual glm::vec3 GetColliderScale();
		//virtual void Update(float) const;
	};


