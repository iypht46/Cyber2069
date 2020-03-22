#pragma once
#include <memory>
#include "Core/EC/Components/Component.hpp"

#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

//Forward Declaration
namespace Physic{ struct Collision; }

class BehaviourScript :public Component {
public:
	BehaviourScript();
	virtual void Init();
	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDisable();
	virtual void OnTriggerEnter(const Physic::Collision col);
	virtual void OnTriggerStay(const Physic::Collision col);
	virtual void OnTriggerExit(const Physic::Collision col);
	virtual void OnCollisionEnter(const Physic::Collision col);
	virtual void OnCollisionStay(const Physic::Collision col);
	virtual void OnCollisionExit(const Physic::Collision col);
	virtual ~BehaviourScript() = 0;

	//serialization
public:
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::base_class<Component>(this));
	}
};

CEREAL_REGISTER_TYPE(BehaviourScript);
