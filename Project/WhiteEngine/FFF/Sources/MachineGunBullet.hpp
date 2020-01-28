#pragma once
#include "Core/EC/Components/BehaviourScript.h"
#include "Core/EC/Components/Transform.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/GameObject.hpp"

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