#pragma once
#include <bitset>
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"

namespace Core
{
#define BOX_VERTICES 4

	enum class ColliderType : int
	{
		INVALID = -1,
		BOX,
		CIRCLE
	};

	using LayerBit = std::bitset<8>;
	class Rigidbody;
	class Transform;

	class Collider : public Component
	{
	protected:
		Transform* m_transform;
		Rigidbody* m_rigidbody;
		bool m_isStatic = true;
		bool m_isTrigger;

		LayerBit m_layerBit;
		ColliderType m_colliderType;
	public:
		
		//Constructor
		Collider(ColliderType col) : m_colliderType(col) {}
		//Destructor
		~Collider() {};
		//Collider Interface
		void SetTrigger(bool);
		ColliderType GetType();
		bool IsStatic();
		//Virtual
		virtual void SetOrient() {};

		//Inherit Interface
		virtual void OnAwake() {}
		virtual void OnEnable() {}
		virtual void OnDisable() {}
		virtual void OnStart() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnFixedUpdate(float dt) {}
		virtual void OnDestroy() {}

	};

	class BoxCollider : public Collider
	{
	private:
		glm::vec2 m_vertices[BOX_VERTICES];
		float m_width;
		float m_height;
	public:
		//Constructor
		BoxCollider() : Collider(ColliderType::BOX) {}
		void SetBox(float, float);
		void SetBox(float, float, Rigidbody*);

		//Box Collider Interface

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


