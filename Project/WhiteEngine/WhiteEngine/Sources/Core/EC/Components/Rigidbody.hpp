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
		float m_orientation;
		float m_mass;
		float m_invMass;
		float m_gravityScale;

		//Pointers to other components
		Collider* m_collider;
		Transform* m_transform;
	public:
		//Rigidbody Interface
		void Init(float, float);
		Rigidbody();
		~Rigidbody();

		//Inherit Interface
		virtual void OnAwake();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnStart();
		virtual void OnUpdate(float dt);
		virtual void OnFixedUpdate(float dt);
		virtual void OnDestroy();

	};
