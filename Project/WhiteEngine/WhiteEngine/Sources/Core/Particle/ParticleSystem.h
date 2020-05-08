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
	enum class DirectionType : int
	{
		AwayFromSpawn = 0,
		ToSpawn,
		Custom
	};

	enum class EmitterShape
	{
		Circle = 0,
		Line
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
		ParticleBehaviour();
		~ParticleBehaviour();
		//create partcle object and translate value from serialization
		virtual void Init();
		virtual void OnEnable();
		virtual void OnUpdate(float dt);
	};

	class ParticleModule {
	public:
		bool isEnabled = true;

		//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				isEnabled
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
		//Circle emittor setting-----------
		bool spawnOnEdge = true;
		float spawnRadius = 100.0f;
		float minEmissionAngle = 0.0f;
		float maxEmissionAngle = 360.0f;
		//Line emittor setting-------------
		float lineLength = 100.0f;
		float lineAngleOffset = 0.0f;
		//=================================

		void SetEmitterShape(EmitterShape shape) {
			emitterShape = shape;
			sr_EmissionShapeAsInt = (int)emitterShape;
		}

		EmitterShape GetEmitterShape() { return emitterShape; }

	private:
		friend class ParticleSystem;
		EmitterShape emitterShape;
		int sr_EmissionShapeAsInt = 0;

		std::shared_ptr<ObjectPool> particlePool;
		int particleInstanceCount;	//total number of particle object in pool

	//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				particleSamples,
				constantParticle,
				burstParticleNumber,
				particleRate,
				sr_EmissionShapeAsInt,
				spawnOnEdge,
				spawnRadius,
				minEmissionAngle,
				maxEmissionAngle,
				lineLength,
				lineAngleOffset
				);
		}
	};

	class ParticleShape :public ParticleModule {
	public:

		//particle shape setting
		//=================================
		bool fixedScale = true;	//randomized size will keep the scale of the object, will use X size to determin the whole scale
		//random---------------------------
		float minXSize = 0.1f;
		float maxXSize = 0.1f;
		float minYSize = 0.1f;
		float maxYSize = 0.1f;
		//not random-----------------------
		//float defaultSize;
		//lifetime modifier----------------
		bool usingLifetimeScaleModifier = false;
		float scale_ModStart = 0.0f; //at what point will modifier take effect
		float scaleModifierPerFrame = 0.95f;
		//=================================

		//particle rotation setting
		//=================================
		//random---------------------------
		float minRotation = 0.0f;
		float maxRotaion = 360.0f;
		//lifetime modifier----------------
		bool usingLifetimeRotationModifier = false;
		float rotation_ModStart = 0.0f; //at what point will modifier take effect
		float rotationSpeed = 90.0f;
		//=================================

		void SetRotationType(DirectionType rotationType) {
			this->rotationType = rotationType;
			sr_rotationTypeAsInt = (int)rotationType;
		}

		DirectionType GetRotationType() { return rotationType; }

	private:
		friend class ParticleSystem;
		DirectionType rotationType;

		//serialization
	public:
		int sr_rotationTypeAsInt = 2;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				minXSize,
				maxXSize,
				minYSize,
				maxYSize,
				usingLifetimeScaleModifier,
				scale_ModStart,
				scaleModifierPerFrame,
				sr_rotationTypeAsInt,
				minRotation,
				maxRotaion,
				usingLifetimeRotationModifier,
				rotation_ModStart,
				rotationSpeed
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
		void serialize(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				minLifeTime,
				maxLifeTime
				);
		}
	};

	class ParticleColor :public ParticleModule {
	public:

		//particle color setting
		//=================================
		//random between these two color
		glm::vec3 Color;	//this is the default starting color
		//lifetime modifier----------------
		bool usingLifeTimeModifier = false;
		float InterpolationStart = 0.0f; //[range 0.0,1.0] at what point in lifetime will the interpolation start to take effect
		float InterpolationEnd = 1.0f; //[range 0.0,1.0] at what point in lifetime will the interpolation finish interpolation
		glm::vec3 Color_Start;	//this is the default starting color
		glm::vec3 Color_End;
		//=================================

		ParticleColor() { Color = glm::vec3(1); Color_Start = glm::vec3(1); Color_End = glm::vec3(1);}

	private:
		friend class ParticleSystem;

		//serialization
	public:

		template<class Archive>
		void serialize(Archive& archive) {
			archive(
				cereal::base_class<ParticleModule>(this),
				Color,
				usingLifeTimeModifier,
				InterpolationStart,
				InterpolationEnd,
				Color_Start,
				Color_End
				);
		}
	};

	class ParticleVelocity :public ParticleModule {
	public:

		//particle speed setting
		//=================================
		float minSpeed = 50.0f;
		float maxSpeed = 50.0f;
		void SetDirectiontype(DirectionType type) { directionType = type; sr_directionTypeAsInt = static_cast<int>(type); };
		DirectionType GetDirectionType() { return directionType; }
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
		void serialize(Archive& archive) {
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
		std::string controllerPath = "none";

		ParticleAnimation() { isEnabled = false; }

		//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
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
	std::string texturePath = "none";//"Sources/Assets/white_square.png";

	//create default modules create objpool (assign partice number), init objpool (instatiate objects), modify particle objject in obj pool according to data
	virtual void Init();

	void SpawnParticle(GameObject*);
	void UpdateMesh();
	void UpdateRigidbody();

	void TriggerBurstEmission(); //only works when set to burst mode
	void ConstantEmit(float dt); //constantly called from update loop

	void LifeTimeModification(float dt); ///+++++++++++ call thi in fac cl too
	std::set<GameObject*>& GetSpawnedParticles();
	void Reset();
	ParticleSystem();
	~ParticleSystem();

private:
	float emitTimer = 0;
	std::set<GameObject*> SpawnedParticles; //used for updating all active particle

	std::vector<std::shared_ptr<GameObject>> Particles; //just to keep reference of all existing particle

	//serialization
	public:
		template<class Archive>
		void serialize(Archive& archive) {
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
