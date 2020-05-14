#include "CameraController.h"

#include "Core/EC/Components/Transform.hpp"
#include "Core/Logger.hpp"
#include "Utility/WhiteMath.h"

#include <glm/glm.hpp>

CameraController* CameraController::_instance = nullptr;

CameraController::CameraController() {
	LimitAreaSize = glm::vec3(3000, 1000, 1);
	LimitAreaOffset = glm::vec3(0, 500, 0);

	if (_instance == nullptr) {
		_instance = this;
	}
}

CameraController* CameraController::GetInstance() {
	if (_instance == nullptr) {
		ENGINE_ERROR("No instance of CameraController Available");
	}
	return _instance;
}


void CameraController::SetTarget(Transform* target) {
	Target = target;

	fixedPos = false;
	following = target != nullptr;
}

void CameraController::SetTarget(glm::vec3 target) {
	TargetPos = target;

	fixedPos = true;
	following = true;
}

void CameraController::StopFollowing() {
	following = false;
}

void CameraController::ShakeCamera(float magnitude, float freq, float duration) {
	shakeMagnitude = magnitude;
	shakeFrequency = freq;
	shakeDuration = duration;

	shaking = true;
	shakeTimer = 0.0f;
	freqTimer = 1.0f / freq;
}

void CameraController::StopShaking() {
	shaking = false;
	shakeTimer = 0.0f;
	freqTimer = 0.0;
}

void CameraController::OnAwake() {
	cam = Graphic::getCamera();
}

void CameraController::OnFixedUpdate(float dt) {
	glm::vec3 movement(0);
	if (following) {
		if (!fixedPos) {
			TargetPos = Target->GetPosition();
		}
		float distance = glm::length((glm::vec2)TargetPos - (glm::vec2)cam->GetCampos());

		float speed = 0;
		if (distance < 10) {
			distance = 0;
			speed = 0.0f;
		}
		else if (distance < DistanceThreshold) {
			speed = glm::clamp(distance / DistanceThreshold, 0.0f, 1.0f) * MaxFollowSpeed;
		}
		else if (distance > DistanceThreshold) {
			speed = MaxFollowSpeed;
		}

		glm::vec2 direction = (glm::vec2)glm::normalize(TargetPos - cam->GetCampos());

		movement = glm::vec3(direction * speed, 0.0f);
		//ENGINE_INFO("dis: {}, speed: {}*{} = {},movement*dt = {},{}", distance, glm::clamp(distance / DistanceThreshold, 0.0f, 1.0f), MaxFollowSpeed, speed, movement.x * dt, movement.y * dt);

	}

	float minX = -(LimitAreaSize.x / 2.0f) + LimitAreaOffset.x;
	float maxX = (LimitAreaSize.x / 2.0f) + LimitAreaOffset.x;
	float minY = -(LimitAreaSize.y / 2.0f) + LimitAreaOffset.y;
	float maxY = (LimitAreaSize.y / 2.0f) + LimitAreaOffset.y;

	glm::vec3 campos = cam->GetCampos() + (movement * dt);

	campos.x = glm::clamp(campos.x, minX, maxX);
	campos.y = glm::clamp(campos.y, minY, maxY);

	//ENGINE_INFO("campos = {},{}", campos.x, campos.y);

	glm::vec3 ShakeOffset(0);
	if (shaking) {
		if (shakeTimer < shakeDuration) {
			if (freqTimer >= (1.0f / shakeFrequency)) {
				float angle = WhiteMath::Rand(0.0f, 2.0f * glm::pi<float>());
				ShakeOffset = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * shakeMagnitude;
				freqTimer = 0.0f;
				//ENGINE_WARN("cam shake {},{}", ShakeOffset.x, ShakeOffset.y);
			}
			else {
				freqTimer += dt;
			}
			shakeTimer += dt;
		}
		else {
			shaking = false;
			shakeTimer = 0.0f;
			freqTimer = 0.0f;
		}
	}

	cam->SetPos(campos + ShakeOffset);
}