#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"
#include "Core/Message/IMessageHandler.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

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
		bool m_isStatic = true;
		float m_density;
		float m_friction = 0.1f;

		COL_STATE m_collisionState = COL_STATE::NONE;
		bool m_hasCollided = false;
	public:
		std::shared_ptr<Transform> m_transform;
		std::shared_ptr<Rigidbody> m_rigidbody;

		//Constructor
		Collider(COLLIDER_TYPE col) 
			: m_colliderType(col), m_density(1.0f) {}
		//Destructor
		~Collider() {};
		//Collider Interface
		COLLIDER_TYPE GetType();
		bool IsStatic();
		COL_STATE GetCollisionState();

		void SetFriction(float friction) { m_friction = friction; }
		float GetFriction() { return m_friction; }

		//Virtual
		virtual void SetOrient() {};
		virtual void Update(float) const {}
		virtual void ComputeMass() {};

		//Message Handling
		virtual void HandleMessage(const Core::Collision&);
		virtual void HandleMessage(const Core::Trigger&);

	//serialization
	private:
		int m_colliderTypeAsInt;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(cereal::base_class<Component>(this),
				m_colliderTypeAsInt,
				m_isStatic,
				m_density,
				m_friction
			);
		}
	};

	CEREAL_REGISTER_TYPE(Collider);
	CEREAL_REGISTER_POLYMORPHIC_RELATION(Core::IMessageHandler, Collider);

	class BoxCollider : public Collider
	{
		friend class Rigidbody;
	private:
		float m_halfWidth;
		float m_halfHeight;
		/*unused var*/glm::vec3 m_colliderScale; //Scale multiplier in halfwidth and halfheight
	public:
		//Constructor
		BoxCollider() : Collider(COLLIDER_TYPE::BOX) {}
		void Init(float, float);
		void Init(float, float, std::shared_ptr<Rigidbody>);
		void ReSize(float, float);
		void ComputeAABB(Physic::AABB&);
		
		//Box Collider Interface
		virtual void ComputeMass();
		virtual glm::vec3 GetColliderScale();

		float GetHw() { return m_halfWidth; }
		float GetHh() { return m_halfHeight; }
		//virtual void Update(float) const;

	//serialization
	private:
		int sr_colliderTypeAsInt;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(cereal::base_class<Collider>(this),
				sr_colliderTypeAsInt,
				m_halfWidth,
				m_halfHeight
			);
		}
	};

	CEREAL_REGISTER_TYPE(BoxCollider);

