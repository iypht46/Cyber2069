#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/Collider.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

//Forward Declaration
class Collider;
class Transform;

class Rigidbody : public Component
{
	friend class BoxCollider;
private:
	//Body Settings
	glm::vec3 m_position = glm::vec3(0);
	glm::vec3 m_velocity = glm::vec3(0);
	glm::vec3 m_acceleration = glm::vec3(0);
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
	virtual void Init();
	void Init(float, float);
	Collider* GetCollider();
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

//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			automass,
			m_mass,
			m_invMass,
			m_gravityScale,
			m_drag
		);
	}
};

CEREAL_REGISTER_TYPE(Rigidbody);