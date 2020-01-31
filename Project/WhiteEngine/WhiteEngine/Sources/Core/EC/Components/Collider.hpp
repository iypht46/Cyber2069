#pragma once
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"
#include "Core/Message/IMessageHandler.hpp"


#define BOX_VERTICES 4

	enum class COLLIDER_TYPE : int {INVALID = -1, BOX, COLNUM };

	enum class COL_STATE : int { NONE = -1, ENTER, STAY, EXIT };

	//Forward Declaration
	class Rigidbody;
	class Transform;
	namespace Physic { struct AABB; class PhysicScene; }

	class Collider : public Component, public Core::IMessageHandler
	{
	protected:
		friend class Physic::PhysicScene;

		COLLIDER_TYPE m_colliderType;
		COL_STATE m_collisionState = COL_STATE::NONE;
		bool m_isStatic = true;
		bool m_isTrigger;
		bool m_hasCollided = false;
		float m_density;
	public:
		Transform* m_transform;
		Rigidbody* m_rigidbody;

		//Constructor
		Collider(COLLIDER_TYPE col) 
			: m_colliderType(col), m_density(1.0f) {}
		//Destructor
		~Collider() {};
		//Collider Interface
		void SetTrigger(bool);
		bool IsTrigger(void);
		COLLIDER_TYPE GetType();
		bool IsStatic();
		COL_STATE GetCollisionState();

		//Virtual
		virtual void SetOrient() {};
		virtual void Update(float) const {}
		virtual void ComputeMass() {};

		//Message Handling
		virtual void HandleMessage(const Core::Collision&);
		virtual void HandleMessage(const Core::Trigger&);

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
		BoxCollider() : Collider(COLLIDER_TYPE::BOX) {}
		void Init(float, float);
		void Init(float, float, Rigidbody*);
		void ComputeAABB(Physic::AABB&);
		
		//Box Collider Interface
		virtual void ComputeMass();
		virtual glm::vec3 GetColliderScale();

		
		//virtual void Update(float) const;
	};


