#pragma once
#include <memory>

#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class MachineGunBullet : public BehaviourScript {
protected:
	Rigidbody* rb;

	Graphic::CameraObject* cam;

	int winWidth;
	int winHeight;
	
public:

	MachineGunBullet() {}
	~MachineGunBullet() {}

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col) override;
	virtual void OnCollisionEnter(const Physic::Collision col) override;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(cereal::base_class<BehaviourScript>(this)

			);
	}
};

CEREAL_REGISTER_TYPE(MachineGunBullet);