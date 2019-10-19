#pragma once

#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/Collider.hpp"

namespace Core
{
	//Forward Declaration
	class Collider;
	class Transform;

	class Rigidbody : public Component
	{
	private:
		//Body Type
		bool m_isKinematic;

		//Body Settings
		glm::vec2 m_position;
		glm::vec2 m_velocity;
		float m_orientation;
		float m_mass;
		float m_gravityScale;

		//Pointers to other components
		Collider* m_collider;
		Transform* m_transform;
	public:
		//Rigidbody Interface
		void Initialize(Collider&);
		void Initialize(float, float);
		void SetKinematic(bool);
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
}
