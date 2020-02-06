#pragma once

#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/Collider.hpp"


	//Forward Declaration
	class Collider;
	class Transform;

	class Rigidbody : public Component
	{
		friend class BoxCollider;
	private:
		//Body Settings
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		glm::vec3 m_acceleration;
		//float m_orientation;

		bool automass = true;
		float m_mass = 1.0f;
		float m_invMass;
		float m_gravityScale;
		float m_drag;

		//Pointers to other components
		Collider* m_collider;
		Transform* m_transform;
	public:
		//Rigidbody Interface
		void Init(float, float);
		Collider* GetCollider();
		void Init(/*float, float*/);
		void SetVelocity(glm::vec3);
		void SetVelocity0Masked(glm::vec3);
		glm::vec3 GetVelocity();
		float GetGravityScale();
		void SetGravityScale(float);
		void AddForce(glm::vec3);
		void AddRelativeForce(glm::vec3);
		void UpdateTransform(float dt);
		void SetMass(float);
		float GetInvMass(void);
		float GetMass(void);
		void SetDrag(float drag);
		float GetDrag();
		Rigidbody();
		~Rigidbody();
	};
