#pragma once

#include <glm/glm.hpp>

#include "Core/EC/Components/BehaviourScript.h"
#include "Graphic/Camera.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

#include "Serialization/glmCereal.h"

struct CameraSetting {
	glm::vec3 LimitAreaSize = glm::vec3(5000, 1500, 1);
	glm::vec3 LimitAreaOffset = glm::vec3(0, 750, 0);

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			CEREAL_NVP(LimitAreaSize),
			CEREAL_NVP(LimitAreaOffset)
		);
	}
};

class Transform;

class CameraController : public BehaviourScript {
private:
	static CameraController* _instance;

	Graphic::CameraObject* cam;

	//tmp vars --------------
	bool following = false;
	bool fixedPos = false;		//if fixed camera will ignore target transform and only follow the set position
	Transform* Target;
	glm::vec3 TargetPos;

	bool shaking = false;
	float shakeMagnitude;
	float shakeDuration;
	float shakeFrequency;
	float shakeTimer;
	float freqTimer;
	//-----------------------

public:
	static CameraController* GetInstance();

	CameraSetting setting;

	float MaxFollowSpeed = 2500.0f;
	float NormalFollowingSpeed = 500.0f;
	float DistanceThreshold = 300.0f;

	glm::vec3 LimitAreaSize;
	glm::vec3 LimitAreaOffset;

	CameraController();

	void ShakeCamera(float magnitude, float freq, float duration);
	void StopShaking();

	void SetTarget(Transform* target);
	void SetTarget(glm::vec3);
	void StopFollowing();

	void SetCameraSetting(CameraSetting setting);
	CameraSetting GetCameraSetting() { return this->setting; }

	virtual void OnAwake() override;
	virtual void OnFixedUpdate(float) override;
	
//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this),
			MaxFollowSpeed,
			DistanceThreshold,
			LimitAreaSize,
			LimitAreaOffset
			);
	}
};

CEREAL_REGISTER_TYPE(CameraController);