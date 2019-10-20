#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Component.hpp"

using namespace std;

class Transform : public Component {
private:
	Transform* parent;
	std::vector<Transform*> children;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_rotation;

	glm::vec3 m_localPosition;
	//glm::vec3 m_localScale;
	float m_localRotation;

	glm::mat4 m_modelMatrix;

	void UpdateWorldPosition();
	//void UpdateScale();
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

	void SetParent(Transform* newParent);

	void SetPosition(glm::vec3 position);
	void SetLocalPosition(glm::vec3 localposition);
	void Translate(glm::vec3 translation);
	void SetScale(glm::vec3 scale);
	//void SetLocalScale(glm::vec3 localscale);
	void SetRotation(float rotation);
	void SetLocalRotation(float localrotation);
	void Rotate(float rotation);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};