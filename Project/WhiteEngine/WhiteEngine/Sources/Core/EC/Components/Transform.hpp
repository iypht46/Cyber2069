#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Component.hpp"

//serialization
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

using namespace std;

class Transform : public Component {
private:
	std::shared_ptr<Transform> parent;
	std::vector<shared_ptr<Transform>> children;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	//rotation in degree
	float m_rotation;

	glm::vec3 m_localPosition;
	glm::vec3 m_localScale;
	float m_localRotation;

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

	glm::vec3 GetPosition();
	glm::vec3 GetLocalPosition();
	glm::vec3 GetScale();
	//glm::vec3 GetLocalScale();
	float GetRotation();
	float GetLocalRotation();
	glm::vec3 Up();
	glm::vec3 Right();

	glm::mat4 GetModelMatrix();

	void SetParent(std::shared_ptr<Transform> newParent);

	std::shared_ptr<Transform> GetChild(int index);

	void SetPosition(glm::vec3 position);
	void SetLocalPosition(glm::vec3 localposition);
	void Translate(glm::vec3 translation);
	void SetScale(glm::vec3 scale);
	void SetLocalScale(glm::vec3 localscale);
	void SetRotation(float rotation);
	void SetLocalRotation(float localrotation);
	void Rotate(float rotation);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();

//serialization
private:
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