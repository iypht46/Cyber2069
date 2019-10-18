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

	class Collider : public Component
	{
	private:
		Transform* m_transform;
		Rigidbody* m_rigidbody;

	protected:
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
		void AttachRigidbody(Rigidbody&);
		ColliderType GetType();
		bool IsStatic();
		//Virtual
		virtual void SetOrient() = 0;

		//Inherit Interface
		virtual void OnAwake() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnFixedUpdate(float dt) = 0;
		virtual void OnDestroy() = 0;

	};

	class BoxCollider : public Collider
	{
	private:
		glm::vec2 m_vertices[BOX_VERTICES];
		glm::vec2 m_size;
	public:
		BoxCollider(glm::vec2);
		BoxCollider(glm::vec2, Rigidbody&);
		//Box Collider Interface


		//Inherit Interface
		virtual void OnAwake() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnFixedUpdate(float dt) = 0;
		virtual void OnDestroy() = 0;
	};
}


