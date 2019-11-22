#pragma once
#include "BehaviourScript.h"
#include "Transform.hpp"
#include "Rigidbody.hpp"
#include "../GameObject.hpp"

#include "Graphic/Camera.hpp"
#include "Graphic/Window.hpp"

class MachineGunBullet : public BehaviourScript {
protected:
	Rigidbody* rb;

	Graphic::CameraObject* cam;

	int winWidth;
	int winHeight;
	
public:

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
};