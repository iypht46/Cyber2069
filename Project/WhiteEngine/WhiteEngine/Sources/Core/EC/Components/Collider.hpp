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
		bool m_isStatic = true;
		bool m_isTrigger;

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
		Rigidbody* GetRigidBody();

		//Virtual
		virtual void SetOrient() {};
		virtual void Update(float) const {}

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
		float m_width;
		float m_height;
	public:
		//Constructor
		BoxCollider() : Collider(ColliderType::BOX) {}
		void Init(float, float);
		void Init(float, float, Rigidbody*);

		//Box Collider Interface
		virtual void Update(float) const;
		

		//Inherit Interface
		virtual void OnAwake();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnStart();
		virtual void OnUpdate(float dt);
		virtual void OnFixedUpdate(float dt);
		virtual void OnDestroy();
	};


