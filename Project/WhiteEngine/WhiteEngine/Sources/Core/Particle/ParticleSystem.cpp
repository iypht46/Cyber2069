#include "ParticleSystem.h"
#include "Utility/WhiteMath.h"

//Partice System ==============================================================================
ParticleSystem::ParticleSystem() {
	//add to factory
	Factory<Component, ParticleSystem>::Add(this);

	//create standard module, these could get replaced when load from prefab
	emitter = std::make_shared<ParticleEmitter>();
	lifetime = std::make_shared<ParticleLifeTime>();
	shape = std::make_shared<ParticleShape>();
	color = std::make_shared<ParticleColor>();
	velocity = std::make_shared<ParticleVelocity>();
	animation = std::make_shared<ParticleAnimation>();

	emitTimer = 0;
}

ParticleSystem::~ParticleSystem() {
	//add to factory
	Factory<Component, ParticleSystem>::Remove(this);
}

void ParticleSystem::Init() {
	//setup system modules

	//lifetime===========================================================================
	//no setup needed

	//Color==============================================================================
	//no setup needed

	//shape==============================================================================
	shape->rotationType = (DirectionType)(shape->sr_rotationTypeAsInt);

	//velocity===========================================================================
	velocity->directionType = (DirectionType)(velocity->sr_directionTypeAsInt);

	//emitter============================================================================
	emitter->particlePool = std::make_shared<ObjectPool>();
	emitter->particlePool->objectCount = emitter->particleSamples;
	//create object and add appropriate cmponent
	while (emitter->particleInstanceCount < emitter->particleSamples) {
		/*use regular ptr to keep it out of the system
		and not get init by the scene, we want to manually initialize it*/
		
		/*its components remains in the system and gets updated still*/
		
		std::shared_ptr<GameObject> particle = std::make_shared<GameObject>();
		Particles.push_back(particle);

		//add essential component
		MeshRenderer* mr = particle->AddComponent<MeshRenderer>();
		Rigidbody* rb = particle->AddComponent<Rigidbody>();

		//add animator (if used)
		if (animation->isEnabled) {
			Animator* anim = particle->AddComponent<Animator>();
			anim->sr_controllerPath = animation->controllerPath;
		}

		//add collider (if used)
		if (velocity->colliding) 
		{
			//set physics layer
			particle->Layer = velocity->PhysicsLayer;

			particle->AddComponent<BoxCollider>()->ReScale(1, 1);
		}

		ParticleBehaviour* pb = particle->AddComponent<ParticleBehaviour>();

		//setup mesh renderer
		mr->SetTexture(texturePath);
		mr->SetLayer(RenderLayer);
		mr->SetReplaceColor(color->Color);


		//setup rigidbody
		rb->SetGravityScale(velocity->gravityScale);
		rb->SetDrag(velocity->drag);


		//init the components manually
		particle->InitComponents();

		particle->SetActive(false);
		
		emitter->particlePool->AddObject(particle.get());
		++emitter->particleInstanceCount;
	}
}

void ParticleSystem::SpawnParticle(GameObject* p) {
	/*if (!p)
		return;*/
	ParticleBehaviour* particle = p->GetComponent<ParticleBehaviour>();

	//set spawn position =========================================================
	float spawndirection = glm::radians(m_gameObject->m_transform->GetRotation() + WhiteMath::Rand(emitter->minEmissionAngle, emitter->maxEmissionAngle));
	glm::vec3 spawnPosition(glm::cos(spawndirection), glm::sin(spawndirection), 0.0);
	//set spawn distance from center;
	if (emitter->spawnOnEdge) {
		spawnPosition *= emitter->spawnRadius;
	}
	else {
		spawnPosition *= WhiteMath::Rand(0.0f, emitter->spawnRadius);
	}
	particle->transform->SetPosition(spawnPosition + m_gameObject->m_transform->GetPosition());

	//setup particle shape ========================================================
	glm::vec3 scale(WhiteMath::Rand(shape->minXSize, shape->maxXSize), WhiteMath::Rand(shape->minYSize, shape->maxYSize), 1.0);
	particle->transform->SetScale(scale);

	//setup particle rotation =====================================================
	float rotation;
	switch (shape->rotationType)
	{
	default:
	case ParticleSystem::DirectionType::AwayFromcenter:
	{
		glm::vec3 direction = particle->transform->GetPosition() - m_gameObject->m_transform->GetPosition();
		rotation = glm::degrees(glm::atan(direction.y / direction.x));
		break;
	}
	case ParticleSystem::DirectionType::ToCenter:
	{
		glm::vec3 direction = m_gameObject->m_transform->GetPosition() - particle->transform->GetPosition();
		rotation = glm::degrees(glm::atan(direction.y / direction.x));
		break;
	}
	case ParticleSystem::DirectionType::Custom:
		rotation = WhiteMath::Rand(shape->minRotation, shape->maxRotaion) + m_gameObject->m_transform->GetRotation();
		break;
	}
	particle->transform->SetRotation(rotation);

	//setup particle lifetime =====================================================
	particle->maxLifetime = WhiteMath::Rand(lifetime->minLifeTime, lifetime->maxLifeTime);

	//set velocity ================================================================
	glm::vec3 direction;
	//set according to direction type
	switch (velocity->directionType)
	{
	default:
	case ParticleSystem::DirectionType::AwayFromcenter:
		direction = particle->transform->GetPosition() - m_gameObject->m_transform->GetPosition();
		break;

	case ParticleSystem::DirectionType::ToCenter:
		direction = m_gameObject->m_transform->GetPosition() - particle->transform->GetPosition();
		break;

	case ParticleSystem::DirectionType::Custom:
		direction = glm::vec3(velocity->customDirection, 0.0f);
		break;
	}

	float speed = WhiteMath::Rand(velocity->minSpeed, velocity->maxSpeed);
	particle->rb->SetVelocity(glm::normalize(direction) * speed);

	//set color ===================================================================
	//if random color

	p->SetActive(true);
}

void ParticleSystem::UpdateMesh()
{
	for (auto gameObj : Particles)
	{
		auto mesh = gameObj->GetComponent<MeshRenderer>();
		if (mesh)
			mesh->SetReplaceColor(color->Color);
	}
}

void ParticleSystem::UpdateRigidbody()
{
	for (auto gameObj : Particles)
	{
		auto rigid = gameObj->GetComponent<Rigidbody>();
		if (rigid)
		{
			rigid->SetGravityScale(velocity->gravityScale);
			rigid->SetDrag(velocity->drag);
		}
	}
}

void ParticleSystem::ConstantEmit(float dt) {
	if (emitter->isEnabled && emitter->constantParticle) {
		emitTimer += dt;
		if (emitTimer > (1.0f / emitter->particleRate)) {
			emitTimer = 0;

			//spawn a single particle
			GameObject* particle = emitter->particlePool->GetGameObject();

			SpawnParticle(particle);

			//store it in active pool
			SpawnedParticles.insert(particle);
		}
	}
}

void ParticleSystem::TriggerBurstEmission() {
	if (emitter->isEnabled) {
		for (int i = 0; i < emitter->burstParticleNumber; ++i) {
			//spawn a single particle
			GameObject* particle = emitter->particlePool->GetGameObject();

			SpawnParticle(particle);

			//store it in active pool
			SpawnedParticles.insert(particle);
		}
	}
}

void ParticleSystem::LifeTimeModification(float dt) {
	std::set<GameObject*> removelist;

	for (GameObject* particle : SpawnedParticles) {
		/*if (!particle)
			continue;*/
		ParticleBehaviour* p = particle->GetComponent<ParticleBehaviour>();

		//if expired
		if (p->lifetime > p->maxLifetime) {
			p->lifetime = 0;
			particle->SetActive(false);

			//add to to-be-remove list
			removelist.insert(particle);
		}
		else {
			//modify shape ============================================

			//modify velocity =========================================
			if (velocity->usingLifetimeModifier) {
				p->rb->SetVelocity(p->rb->GetVelocity() * velocity->lifetimeSpeedModifier);
			}
		}
	}

	//remove expired particle from the set
	for (GameObject* rm : removelist) {
		SpawnedParticles.erase(rm);
	}
}

std::set<GameObject*>& ParticleSystem::GetSpawnedParticles()
{
	return SpawnedParticles;
}

void ParticleSystem::Reset()
{
	emitter->particlePool.reset();
	emitter->particleInstanceCount = 0;
	Particles.clear();
	SpawnedParticles.clear();
}


//Partice Bahaviour ==============================================================================
void ParticleSystem::ParticleBehaviour::Init() {
	transform = m_gameObject->m_transform.get();
	renderer = m_gameObject->GetComponent<MeshRenderer>();
	rb = m_gameObject->GetComponent<Rigidbody>();
	colllider = m_gameObject->GetComponent<Collider>();
	anim = m_gameObject->GetComponent<Animator>();
}

void ParticleSystem::ParticleBehaviour::OnEnable() {
	lifetime = 0;
}

void ParticleSystem::ParticleBehaviour::OnUpdate(float dt) {
	lifetime += dt;
}