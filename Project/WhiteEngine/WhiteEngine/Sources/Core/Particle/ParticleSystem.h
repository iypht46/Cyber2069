#pragma once

#include <vector>
#include <set>

#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Utility/ObjectPool.h"
#include "glm/glm.hpp"

//serailization
#include "Serialization/glmCereal.h"
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

class ParticleSystem :public Component {
public:
	enum class DirectionType
	{
		AwayFromcenter = 0,
		ToCenter,
		Custom
	};

	class ParticleBehaviour : public BehaviourScript {
	private:
		friend class ParticleSystem;

		Transform* transform;
		MeshRenderer* renderer;
		Rigidbody* rb;
		Collider* colllider;
		Animator* anim;

		float maxLifetime = 0;
		float lifetime = 0;

	public:
		virtual void Init();
		virtual void OnEnable();
		virtual void OnUpdate(float dt);
	};

	class ParticleModule {
	public:
		bool isEnabled = false;

		//serialization
	public:
		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				isEnabled
				);
		}
	};

	class ParticleLifeTime :public ParticleModule {
	public:
		float minLifeTime = 1.5f;
		float maxLifeTime = 1.5f;

	//serialization
	public:
		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				minLifeTime,
				maxLifeTime
				);
		}
	};

	class ParticleShape :public ParticleModule {
	public:

		//particle shape setting
		//=================================
		//bool randomSize;
		//random---------------------------
		float minXSize = 10.0f;
		float maxXSize = 10.0f;
		float minYSize = 10.0f;
		float maxYSize = 10.0f;
		//not random-----------------------
		//float defaultSize;
		//lifetime modifier----------------
		bool usingLifetimeModifier = false;
		float shape_ModStart = 0.0f; //at what point will modifier take effect
		float sizeXModifierbyLifeTime = 0.95f;
		float sizeYModifierbyLifeTime = 0.95f;
		//=================================

		//particle rotation setting
		//=================================
		//bool randomSize;
		DirectionType rotationType;
		//random---------------------------
		float minRotation = 0.0f;
		float maxRotaion = 360.0f;
		//lifetime modifier----------------
		bool usingLifetimeRotModifier = false;
		float rotationModifier = 0.0f;
		//=================================

		//serialization
	public:
		int sr_rotationTypeAsInt = 2;

		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				minXSize,
				maxXSize,
				minYSize,
				maxYSize,
				usingLifetimeModifier,
				shape_ModStart,
				sizeXModifierbyLifeTime,
				sizeYModifierbyLifeTime,
				sr_rotationTypeAsInt,
				minRotation,
				maxRotaion
				);
		}
	};

	class ParticleEmitter :public ParticleModule {
	public:

		//spawn setting
		//=================================
		int particleSamples = 20;
		bool constantParticle = true;
		//burst particle spawning----------
		int burstParticleNumber = 5;
		//constant particle spawning-------
		float particleRate = 1.5f;
		//=================================

		//spawn shape setting
		//=================================
		bool spawnOnEdge = true;
		float spawnRadius = 100.0f;
		float minEmissionAngle = 0.0f;
		float maxEmissionAngle = 360.0f;
		//=================================

	private:
		friend class ParticleSystem;
		std::shared_ptr<ObjectPool> particlePool;
		int particleInstanceCount;	//total number of particle object in pool

	//serialization
	public:
		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				particleSamples,
				constantParticle,
				burstParticleNumber,
				particleRate,
				spawnOnEdge,
				spawnRadius,
				minEmissionAngle,
				maxEmissionAngle
				);
		}
	};

	class ParticleColor :public ParticleModule {
	public:

		//particle color setting
		//=================================
		string ColorHex;
		//lifetime modifier----------------
		bool useLifeTimeMod;
		//=================================

	private:
		friend class ParticleSystem;
		glm::vec3 color;
	};

	class ParticleVelocity :public ParticleModule {
	public:

		//particle speed setting
		//=================================
		float minSpeed = 50.0f;
		float maxSpeed = 50.0f;
		void SetDirectiontype(DirectionType type) { directionType = type; sr_directionTypeAsInt = static_cast<int>(type); };
		//custom direction-----------------
		glm::vec2 customDirection;
		//=================================

		//velocity setting
		//=================================
		bool usingLifetimeModifier = false;
		float lifetimeSpeedModifier = 1.0f;
		//=================================

		//physics setting
		//=================================
		float gravityScale = 0.0f;
		float drag = 0.0f;
		bool colliding = false;
		string PhysicsLayer = "Default";  //only used if colliding = true
		//=================================

	private:
		friend class ParticleSystem;
		DirectionType directionType;

	//serialization
	public:
		int sr_directionTypeAsInt = 1;

		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				sr_directionTypeAsInt,
				minSpeed,
				maxSpeed,
				customDirection,
				usingLifetimeModifier,
				lifetimeSpeedModifier,
				gravityScale,
				drag,
				colliding,
				PhysicsLayer
				);
		}
	};

	class ParticleAnimation :public ParticleModule {
	public:
		std::string controllerPath = "";
	
	//serialization
	public:
		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				controllerPath
				);
		}
	};

public:
	std::shared_ptr<ParticleEmitter> emitter;
	std::shared_ptr<ParticleLifeTime> lifetime;
	std::shared_ptr<ParticleShape> shape;
	std::shared_ptr<ParticleColor> color;
	std::shared_ptr<ParticleVelocity> velocity;
	std::shared_ptr<ParticleAnimation> animation;

	int RenderLayer = 1;
	std::string texturePath = "Sources/Assets/white_square.png";

	//create default modules create objpool (assign partice number), init objpool (instatiate objects), modify particle objject in obj pool according to data
	virtual void Init();

	void SpawnParticle(GameObject*);

	void TriggerBurstEmission(); //only works when set to burst mode
	void ConstantEmit(float dt); //constantly called from update loop

	void LifeTimeModification(float dt); ///+++++++++++ call thi in fac cl too

	ParticleSystem();

private:
	float emitTimer = 0;
	std::set<GameObject*> SpawnedParticles; //used for updating all active particle
	
	std::vector<std::shared_ptr<GameObject>> Particles; //just to keep reference of all existing particle

	//serialization
	public:
		template<class Archive>
		void serialization(Archive& archive) {
			archive(
				cereal::base_class<Component>(this),
				emitter,
				lifetime,
				shape,
				color,
				velocity,
				animation,
				RenderLayer,
				texturePath
				);
		}
};

CEREAL_REGISTER_TYPE(ParticleSystem);
CEREAL_REGISTER_TYPE(ParticleSystem::ParticleBehaviour);

//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleEmitter);
//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleLifeTime);
//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleShape);
//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleColor);
//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleVelocity);
//CEREAL_REGISTER_TYPE(ParticleSystem::ParticleAnimation);