#pragma once
#include <vector>
#include <glfw3.h>
#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

#include "Component.hpp"

class Transform :public Component {
private:
	Transform* parent;
	std::vector<Transform*> children;

	glm::vec3 m_position;
	glm::vec3 m_scale;
	float m_rotation;

	glm::vec3 m_localPosition;
	glm::vec3 m_localScale;
	float m_localRotation;

	glm::mat4 m_modelMatrix;
public:
	glm::vec3 GetPosition();
	glm::vec3 GetScale();
	float GetRotation();
	glm::vec3 Up();
	glm::vec3 Right();

	void SetPosition(glm::vec3& position);
	void Translate(glm::vec3& translation);
	void SetScale(glm::vec3 scale);
	void SetRotation(float rotation);
	void Rotate(float rotation);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};