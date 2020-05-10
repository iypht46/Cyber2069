#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Core/Factory.h"

//serialization
#include "Serialization/glmCereal.h"
#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

using namespace std;

class Transform : public Component, public std::enable_shared_from_this<Transform> 
{
private:
	std::weak_ptr<Transform> parent;
	std::vector<weak_ptr<Transform>> children;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	//rotation in degree
	float m_rotation = 0.0f;

	glm::vec3 m_localPosition;
	glm::vec3 m_localScale;
	float m_localRotation = 0.0f;

	static float maxParallaxDistance;

	//runtime rendering matrix
	glm::mat4 m_modelMatrix;

	void UpdateWorldPosition();
	void UpdateScale();
	void UpdateRotation();
	//updatelocalposiitn
	//updatelocalscale
	//updatelocalrotation
public:
	Transform();
	~Transform();

	glm::vec3 GetPosition();
	glm::vec3 GetLocalPosition();
	glm::vec3 GetScale();
	//glm::vec3 GetLocalScale();
	float GetRotation();
	float GetLocalRotation();
	glm::vec3 Up();
	glm::vec3 Right();

	float GetParallaxValue();
	glm::mat4 GetModelMatrix();

	void SetParent(std::weak_ptr<Transform> newParent);
	bool HasParent();

	Transform* GetChild(int index);
	unsigned int GetChildCount();

	void SetPosition(glm::vec3 position);
	void SetLocalPosition(glm::vec3 localposition);
	void Translate(glm::vec3 translation);
	void SetScale(glm::vec3 scale);
	void SetLocalScale(glm::vec3 localscale);
	void SetRotation(float rotation);
	void SetLocalRotation(float localrotation);
	void Rotate(float rotation);

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			m_position,
			m_scale,
			m_rotation,
			m_localPosition,
			m_localScale,
			m_localRotation,
			cereal::defer(parent),
			cereal::defer(children)
			);
	}
};

CEREAL_REGISTER_TYPE(Transform);
