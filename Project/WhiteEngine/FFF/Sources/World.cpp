//System Headers
#include <stdlib.h>
#include <time.h>

#include "World.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Graphic/Camera.hpp"
#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "SceneManagement/SceneManager.h"

#include "Physic/PhysicScene.hpp"
#include "Physic/Collision.hpp"

#include "Core/Factory.h"
#include "Core/FactoryCollection.h"
#include "Core/EC/GameObject.hpp"

#include "Utility/ObjectPool.h"

//component headers might not necessary anymore
#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"
#include "Core/EC/Components/TextRenderer.hpp"
#include "Core/Particle/ParticleSystem.h"

#include "Enemy.hpp"
#include "EnemyBehaviours.h"
#include "PlayerController.hpp"
#include "EnemySpawner.hpp"
#include "GameController.hpp"
#include "Weapon.hpp"
#include "EquipmentManager.hpp"

using SceneManagement::Instantiate;

namespace World
{
	//Constants
	constexpr float c_maxFrameRate = 60.0f;
	constexpr float c_targetDT = 1.0f / c_maxFrameRate;
	bool g_isDebug;

	//Game Info Var
	static GameInfo* g_gameInfo;
	static Graphic::CameraObject* cam = Graphic::getCamera();


	//======================================
	//TESTING ONLY, DON'T FORGET TO REMOVE v
	//======================================
	ObjectPool* BulletPool;

	GameObject* title;
	std::shared_ptr<GameObject> Rabbit;
	GameObject* Bg1;
	GameObject* Bg2;
	GameObject* platform;
	GameObject* queen;

	GameObject* gamecontroller;
	GameObject* ui_ScoreText;
	GameObject* ui_HPbar;
	GameObject* ui_StaminaBar;

	GameObject* Spawner;

	std::shared_ptr<AnimationController> EnemCon;
	std::shared_ptr<AnimationController> RabbitController;
	std::shared_ptr<AnimationController> BomberAnimController;
	std::shared_ptr<AnimationController> queenAnimControl;


	//GameObject** test;

	Physic::PhysicScene* g_physicScene;

	bool running;
	int Delay = 0;
	int Delay2 = 0;
	int enemyNum = 10, platformNum = 5;
	int rand_AD;
	int rand_WS;

	float parlx1 = 0.3f;
	float parlx2 = 0.5f;

#define MOVE_SPEED 300.0f
#define MOVE_SPEED_ENEM 100.0f
#define CHAR_SIZE 120.0f
#define ENEMY_SIZE 40.0f

	//====================================

	//Physic Scene
	//static PhysicScene* g_physicScene;

	void DebugInput(float dt)
	{

		if (Input::GetKeyHold(Input::KeyCode::KEY_I))
		{
			cam->Zoom(1.0f * dt);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_O))
		{
			cam->Zoom(-1.0f * dt);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Q))
		{
			Rabbit->m_transform->Rotate(1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_E))
		{
			Rabbit->m_transform->Rotate(-1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Z))
		{
			Rabbit->m_transform->SetScale(Rabbit->m_transform->GetScale() + glm::vec3(1, 0, 0));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_C))
		{
			Rabbit->m_transform->SetScale(Rabbit->m_transform->GetScale() + glm::vec3(-1, 0, 0));

		}

		//if (Input::GetKeyHold(Input::KeyCode::KEY_N))
		//{
		//	Bg2->GetComponent<SoundPlayer>()->IncreaseVolume();
		//}

		//if (Input::GetKeyHold(Input::KeyCode::KEY_M))
		//{
		//	Bg2->GetComponent<SoundPlayer>()->DecreaseVolume();
		//}
	}

	void Init(void)
	{
		srand(time(NULL));
		g_gameInfo = &(GameInfo::GetInstance());
		g_isDebug = false;
		//Initialize All System
		//Core

		//Runtime
		Core::Logger::Init();
		Graphic::Init();

		//Input
		//Bool for debugging
		Input::Init(false);

		//Physics
		g_physicScene = new Physic::PhysicScene();

		ENGINE_WARN("Engine Initialized");

		SceneManagement::ActiveScene = std::make_unique<SceneManagement::Scene>();
		ENGINE_WARN("Made Temp Scene");

		//Add Physic
		//Set name to layer
		Physic::PhysicScene::GetInstance()->SetLayerName("Player", Physic::Layer::PHYSIC_LAYER_1);
		Physic::PhysicScene::GetInstance()->SetLayerName("Enemy", Physic::Layer::PHYSIC_LAYER_2);
		Physic::PhysicScene::GetInstance()->SetLayerName("Platform", Physic::Layer::PHYSIC_LAYER_3);
		Physic::PhysicScene::GetInstance()->SetLayerName("Bullet", Physic::Layer::PHYSIC_LAYER_4);
		Physic::PhysicScene::GetInstance()->SetLayerName("GroundEnemy", Physic::Layer::PHYSIC_LAYER_5);
		Physic::PhysicScene::GetInstance()->SetLayerName("EnemyBullet", Physic::Layer::PHYSIC_LAYER_6);
		Physic::PhysicScene::GetInstance()->SetLayerName("Default", Physic::Layer::PHYSIC_LAYER_7);
		//Set collision between layer
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Player", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("GroundEnemy", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("EnemyBullet", "Platform", Physic::RESOLVE_TYPE::COLLISION);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Player", "Enemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "Enemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("Bullet", "GroundEnemy", Physic::RESOLVE_TYPE::TRIGGER);
		Physic::PhysicScene::GetInstance()->SetLayerCollisions("EnemyBullet", "Player", Physic::RESOLVE_TYPE::TRIGGER);

		//this point is where it all start =========================================
		
		title = Instantiate().get();
		title->AddComponent<MeshRenderer>();
		title->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		title->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/mockup_title.jpg");
		title->GetComponent<MeshRenderer>()->SetLayer(-1);
		
		title->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth(), Graphic::Window::GetHeight(), 1.0f));

		//GameObject
		enemyNum = 10;

		Rabbit = Instantiate();
		Rabbit->Layer = "Player";
		Bg2 = Instantiate().get();
		Bg1 = Instantiate().get();
		platform = Instantiate().get();
		platform->Layer = "Platform";

		gamecontroller = Instantiate().get();
		ui_ScoreText = Instantiate().get();
		ui_HPbar = Instantiate().get();
		ui_StaminaBar = Instantiate().get();

		Spawner = Instantiate().get();

		//Add Renderer

		while (!Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			Input::Update();

			Graphic::Render();

			if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
			{
				title->SetActive(false);
			}
		}

		ui_ScoreText->AddComponent<TextRenderer>();
		ui_ScoreText->GetComponent<TextRenderer>()->LoadFont("Sources/Assets/Orbitron-Regular.ttf", 50);
		ui_ScoreText->GetComponent<TextRenderer>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		ui_ScoreText->m_transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		ui_ScoreText->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 50.0f, (Graphic::Window::GetHeight() / -2) + 50.0f, 1.0f));

		ui_HPbar->AddComponent<MeshRenderer>();
		ui_HPbar->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		ui_HPbar->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Red.jpg");
		ui_HPbar->GetComponent<MeshRenderer>()->SetUI(true);
		ui_HPbar->GetComponent<MeshRenderer>()->SetLayer(10);
		ui_HPbar->m_transform->SetScale(glm::vec3(500.0f, 40.0f, 1.0f));
		ui_HPbar->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 280.0f, (Graphic::Window::GetHeight() / 2) - 40.0f, 1.0f));

		ui_StaminaBar->AddComponent<MeshRenderer>();
		ui_StaminaBar->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		ui_StaminaBar->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Blue.jpg");
		ui_StaminaBar->GetComponent<MeshRenderer>()->SetUI(true);
		ui_StaminaBar->GetComponent<MeshRenderer>()->SetLayer(10);
		ui_StaminaBar->m_transform->SetScale(glm::vec3(500.0f, 20.0f, 1.0f));
		ui_StaminaBar->m_transform->SetPosition(glm::vec3((Graphic::Window::GetWidth() / -2) + 280.0f, (Graphic::Window::GetHeight() / 2) - 80.0f, 1.0f));

		Bg2->AddComponent<MeshRenderer>();
		Bg2->GetComponent<MeshRenderer>()->SetLayer(-2);
		Bg2->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg2->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer2.png");
		//Bg2->GetComponent<MeshRenderer>()->SetLayer(0);

		Bg1->AddComponent<MeshRenderer>();
		Bg1->GetComponent<MeshRenderer>()->SetLayer(-1);
		Bg1->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg1->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer1.png");
		//Bg1->GetComponent<MeshRenderer>()->SetLayer(1);

		Bg2->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));
		Bg1->m_transform->SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));

		Bg1->m_transform->SetPosition(glm::vec3(0, -300, 0));
		Bg2->m_transform->SetPosition(glm::vec3(0, -300, 0));

		//Add Animator
		std::shared_ptr<Animation> Idle = std::make_shared<Animation>();

		Idle->setStartPosition(0, 0);
		Idle->setEndPosition(6, 0);
		Idle->setSpeedMultiplier(1);

		std::shared_ptr<Animation> Running = std::make_shared<Animation>();

		Running->setStartPosition(0, 1);
		Running->setEndPosition(4, 1);
		Running->setSpeedMultiplier(2);

		std::shared_ptr<Animation> Dashing = std::make_shared<Animation>();

		Dashing->setStartPosition(0, 4);
		Dashing->setEndPosition(2, 4);
		Dashing->setSpeedMultiplier(1);

		std::shared_ptr<Animation> Jumping = std::make_shared<Animation>();

		Jumping->setStartPosition(0, 3);
		Jumping->setEndPosition(3, 3);
		Jumping->setSpeedMultiplier(1);

		std::shared_ptr<Animation> falling = std::make_shared<Animation>();

		falling->setStartPosition(3, 3);
		falling->setEndPosition(3, 3);
		falling->setSpeedMultiplier(1);

		RabbitController = std::make_shared<AnimationController>();
		RabbitController->setSheetSize(glm::vec2(7, 5));

		ENGINE_INFO("making rabbbit animcon");
		RabbitController->AddState(Idle, true);
		RabbitController->AddState(Running, true);
		RabbitController->AddState(Dashing, false);
		RabbitController->AddState(Jumping, false);
		RabbitController->AddState(falling, false);

		Rabbit->AddComponent<Animator>();
		Rabbit->GetComponent<Animator>()->AssignController(RabbitController);
		Rabbit->GetComponent<Animator>()->setCurrentState(0);
		Rabbit->GetComponent<Animator>()->setFramePerSec(12);


		Rabbit->AddComponent<MeshRenderer>();
		Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
		Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_PlayerBody_Vversion03.png");
		Rabbit->GetComponent<MeshRenderer>()->SetLayer(0);
		Rabbit->GetComponent<MeshRenderer>()->SetReplaceColor(glm::vec3(1.0f, 1.0f, 1.0f));

		//Set Transform
		Rabbit->m_transform->SetScale(glm::vec3(CHAR_SIZE, CHAR_SIZE, 1));
		Rabbit->m_transform->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));

		Rabbit->AddComponent<BoxCollider>();

		//Add Rigidbody
		Rabbit->AddComponent<Rigidbody>();
		Rabbit->GetComponent<Rigidbody>()->SetDrag(0.01f);

		Rabbit->GetComponent<BoxCollider>()->m_rigidbody = Rabbit->GetComponent<Rigidbody>();
		Rabbit->GetComponent<BoxCollider>()->ReSize(25, 25);

		//Behavior Script
		Rabbit->AddComponent<HPsystem>();
		Rabbit->GetComponent<HPsystem>()->SetMaxHP(100);
		Rabbit->AddComponent<PlayerController>();

		platform->AddComponent<MeshRenderer>();
		platform->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		platform->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/platform01.png");
		platform->GetComponent<MeshRenderer>()->SetLayer(3);
		platform->m_transform->SetScale(glm::vec3(800, 20, 1));
		platform->AddComponent<BoxCollider>()->ReScale(1, 1);

		//for (int i = 0; i < platformNum; i++)
		//{
		//	platform[i] = Instantiate().get();
		//	platform[i]->AddComponent<MeshRenderer>();
		//	platform[i]->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		//	platform[i]->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/platform01.png");
		//	platform[i]->GetComponent<MeshRenderer>()->SetLayer(3);
		//	platform[i]->m_transform->SetScale(glm::vec3(400, 20, 1));
		//	platform[i]->AddComponent<BoxCollider>()->Init(200, 5);
		//	g_physicScene->Add(platform[i]->GetComponent<BoxCollider>(), "Platform");
		//}

		//platform[1]->m_transform->SetPosition(glm::vec3(300, 300, 0));
		//platform[2]->m_transform->SetPosition(glm::vec3(-300, 300, 0));
		//platform[3]->m_transform->SetPosition(glm::vec3(-300, -300, 0));
		//platform[4]->m_transform->SetPosition(glm::vec3(300, -300, 0));

		gamecontroller->AddComponent<GameController>();
		gamecontroller->GetComponent<GameController>()->player = Rabbit;
		gamecontroller->GetComponent<GameController>()->AssignScoreText(ui_ScoreText);
		gamecontroller->GetComponent<GameController>()->AssignHPbar(ui_HPbar);
		gamecontroller->GetComponent<GameController>()->AssignStaminabar(ui_StaminaBar);
		


		EnemCon = std::make_shared<AnimationController>();

		std::shared_ptr<GameObject> wp_MachineGun = Instantiate();
		wp_MachineGun->AddComponent<MeshRenderer>();
		wp_MachineGun->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
		wp_MachineGun->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_shoot.png");
		wp_MachineGun->GetComponent<MeshRenderer>()->SetLayer(3);


		wp_MachineGun->AddComponent<MachineGun>();

		wp_MachineGun->m_transform->SetParent(Rabbit->m_transform);

		wp_MachineGun->m_transform->SetScale(glm::vec3(70.0f, 70.0f, 1.0f));

		gamecontroller->AddComponent<EquipmentManager>();
		gamecontroller->GetComponent<EquipmentManager>()->AssignWeaponToManager(wp_MachineGun);
		gamecontroller->GetComponent<EquipmentManager>()->AssignPlayer(Rabbit);
		gamecontroller->GetComponent<EquipmentManager>()->InitData();

		gamecontroller->GetComponent<EquipmentManager>()->Unlock_WEAPON(WEAPON_TYPE::WEAPON_MACHINEGUN);
		gamecontroller->GetComponent<EquipmentManager>()->AddPlayerWeapon(WEAPON_TYPE::WEAPON_MACHINEGUN);


		//std::shared_ptr<Animation> Fly = std::make_shared<Animation>();
		//Fly->setStartPosition(0, 0);
		//Fly->setEndPosition(5, 0);
		//Fly->setSpeedMultiplier(3);

		//EnemCon->setSheetSize(glm::vec2(6, 1));

		//EnemCon->AddState(Fly, true);


		Serialization::LoadObject(*EnemCon, "flyer.animcon");

		ENGINE_INFO("Flyer creation==========================================================");
		{
			/*
			GameObject* flyer = Instantiate().get();
			flyer->Layer = "Enemy";
			flyer->AddComponent<MeshRenderer>();
			flyer->GetComponent<MeshRenderer>()->SetLayer(1);
			flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
			flyer->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Flyer_Vversion01.png");

			flyer->AddComponent<Rigidbody>();
			flyer->GetComponent<Rigidbody>()->Init(15, 15);

			flyer->AddComponent<HPsystem>();

			//charger tmp
			//flyer->AddComponent<GroundPatrol>();
			//flyer->AddComponent<GroundDash>();
			//flyer->AddComponent<Charger>()->targetDetectionRange = 300.0f;
			//flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

			//spitter tmp
			//flyer->AddComponent<ObjectShooter>();
			//flyer->AddComponent<Spitter>()->targetDetectionRange = 1000.0f;
			//flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

			//tank tmp
			//flyer->AddComponent<GroundPatrol>()->SetSpeed(50.0f);
			//flyer->AddComponent<Tank>();
			//flyer->GetComponent<Rigidbody>()->SetGravityScale(1.0f);
			//flyer->GetComponent<HPsystem>()->SetMaxHP(50.0);

			//og flyer
			flyer->AddComponent<AirFollowing>();
			flyer->AddComponent<Flyer>();
			flyer->GetComponent<Rigidbody>()->SetGravityScale(0.00000001f);

			flyer->AddComponent<Animator>();
			flyer->GetComponent<Animator>()->setFramePerSec(12);

			flyer->GetComponent<Animator>()->sr_controllerPath = "flyer.animcon";

			flyer->m_transform->SetScale(glm::vec3(50, 50, 1));

			flyer->SetActive(false);

			Serialization::SaveObject(*flyer, "Flyer.prefab");
			*/
		}

		ObjectPool* FlyerPool = Instantiate()->AddComponent<ObjectPool>();

		FlyerPool->prefabObjectPath = "Flyer.prefab";
		FlyerPool->objectCount = 10;

		gamecontroller->GetComponent<GameController>()->AddPool(FlyerPool, POOL_TYPE::ENEMY_CHARGER);

		//ENGINE_INFO("Charger Creation==========================================================");
		//for (int i = 0; i < 1; i++)
		//{

		//	GameObject* flyer = Instantiate().get();
		//	flyer->Layer = "Enemy";
		//	flyer->AddComponent<MeshRenderer>();
		//	flyer->GetComponent<MeshRenderer>()->SetLayer(1);
		//	flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
		//	flyer->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Flyer_Vversion01.png");

		//	flyer->AddComponent<Rigidbody>();
		//	flyer->GetComponent<Rigidbody>()->Init(15, 15);
		//	flyer->GetComponent<Rigidbody>()->SetGravityScale(0.00000001f);

		//	flyer->AddComponent<HPsystem>();
		//	flyer->AddComponent<AirFollowing>();
		//	flyer->AddComponent<Flyer>();

		//	//change to set target instead
		//	//flyer->GetComponent<Flyer>()->Init(Rabbit->m_transform.get());

		//	//no need in actual engine
		//	flyer->GetComponent<AirFollowing>()->OnAwake();

		//	flyer->AddComponent<Animator>();
		//	flyer->GetComponent<Animator>()->setFramePerSec(12);

		//	flyer->GetComponent<Animator>()->sr_controllerPath = "flyer.animcon";

		//	flyer->m_transform->SetScale(glm::vec3(50, 50, 1));

		//	flyer->SetActive(false);

		//	Serialization::SaveObject(*flyer, "Flyer.prefab");

		//	flyer->GetComponent<Enemy>()->SetTarget(Rabbit->m_transform.get());

		//	FlyerPool->AddObject(flyer);
		//}

		std::shared_ptr<Animation> BomberIdle = std::make_shared<Animation>();

		BomberIdle->setStartPosition(0, 0);
		BomberIdle->setEndPosition(4, 0);
		BomberIdle->setSpeedMultiplier(1);

		std::shared_ptr<Animation> BomberCharging = std::make_shared<Animation>();
		BomberCharging->setStartPosition(0, 1);
		BomberCharging->setEndPosition(2, 1);
		BomberCharging->setSpeedMultiplier(1);

		std::shared_ptr<Animation> BomberDashing = std::make_shared<Animation>();
		BomberDashing->setStartPosition(3, 1);
		BomberDashing->setEndPosition(4, 1);
		BomberDashing->setSpeedMultiplier(1);

		std::shared_ptr<Animation> BomberExplode = std::make_shared<Animation>();
		BomberExplode->setStartPosition(0, 2);
		BomberExplode->setEndPosition(11, 2);
		BomberExplode->setSpeedMultiplier(1);

		std::shared_ptr<Animation> BomberDie = std::make_shared<Animation>();
		BomberDie->setStartPosition(0, 3);
		BomberDie->setEndPosition(4, 3);
		BomberDie->setSpeedMultiplier(1);

		BomberAnimController = std::make_shared<AnimationController>();
		BomberAnimController->setSheetSize(glm::vec2(12, 4));
		BomberAnimController->AddState(BomberIdle, true);
		BomberAnimController->AddState(BomberCharging, false);
		BomberAnimController->AddState(BomberDashing, true);
		BomberAnimController->AddState(BomberExplode, false);
		BomberAnimController->AddState(BomberDie, false);

		//bomber
		ObjectPool* BomberPool = Instantiate()->AddComponent<ObjectPool>();
		for (int i = 0; i < 0; i++)
		{
			GameObject* bomber = Instantiate().get();
			bomber->Layer = "Enemy";
			bomber->AddComponent<MeshRenderer>();
			bomber->GetComponent<MeshRenderer>()->CreateMesh(12, 4);
			bomber->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Bomber_V[version01].png");

			bomber->AddComponent<Animator>();
			bomber->GetComponent<Animator>()->AssignController(BomberAnimController);
			bomber->GetComponent<Animator>()->setCurrentState(0);
			bomber->GetComponent<Animator>()->setFramePerSec(12);

			bomber->AddComponent<Rigidbody>();
			bomber->GetComponent<Rigidbody>()->Init(15, 15);
			bomber->GetComponent<Rigidbody>()->SetGravityScale(0.000001);

			bomber->AddComponent<HPsystem>();
			bomber->AddComponent<AirFollowing>();
			bomber->AddComponent<AirDash>();
			bomber->AddComponent<Bomber>();
			bomber->AddComponent<Explosion>();
			//bomber->GetComponent<Bomber>()->Init(Rabbit->m_transform.get());

			bomber->GetComponent<AirFollowing>()->OnAwake();
			bomber->GetComponent<AirDash>()->OnAwake();

			bomber->m_transform->SetScale(glm::vec3(50, 50, 1));

			bomber->SetActive(false);
			BomberPool->AddObject(bomber);
		}

		gamecontroller->GetComponent<GameController>()->AddPool(BomberPool, POOL_TYPE::ENEMY_BOMBER);


		ENGINE_INFO("Creating Bullet");
		//machinegun bullet
		BulletPool = new ObjectPool();
		for (int i = 0; i < 20; i++)
		{
			GameObject* Bullet = Instantiate().get();
			Bullet->Layer = "Bullet";
			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->SetLayer(2);
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_bullet.png");

			Bullet->AddComponent<Rigidbody>();
			Bullet->GetComponent<Rigidbody>()->Init(7, 7);
			Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

			Bullet->AddComponent<ParticleSystem>();
			Bullet->GetComponent<ParticleSystem>()->texturePath = "Sources/Assets/yellow_square.png";
			Bullet->GetComponent<ParticleSystem>()->emitter->isEnabled = true;
			Bullet->GetComponent<ParticleSystem>()->emitter->constantParticle = false;
			Bullet->GetComponent<ParticleSystem>()->emitter->burstParticleNumber = 3;
			Bullet->GetComponent<ParticleSystem>()->emitter->particleSamples = 3;
			Bullet->GetComponent<ParticleSystem>()->emitter->minEmissionAngle = 135;
			Bullet->GetComponent<ParticleSystem>()->emitter->maxEmissionAngle = 225;
			Bullet->GetComponent<ParticleSystem>()->emitter->spawnRadius = 0.1f;
			Bullet->GetComponent<ParticleSystem>()->velocity->minSpeed = 300.0f;
			Bullet->GetComponent<ParticleSystem>()->velocity->maxSpeed = 300.0f;
			Bullet->GetComponent<ParticleSystem>()->velocity->sr_directionTypeAsInt = 0;
			Bullet->GetComponent<ParticleSystem>()->velocity->gravityScale = 2;
			Bullet->GetComponent<ParticleSystem>()->lifetime->minLifeTime = 0.2f;
			Bullet->GetComponent<ParticleSystem>()->lifetime->maxLifeTime = 0.2f;
			Bullet->GetComponent<ParticleSystem>()->shape->minXSize = 4.5f;
			Bullet->GetComponent<ParticleSystem>()->shape->maxXSize = 4.5f;
			Bullet->GetComponent<ParticleSystem>()->shape->minYSize = 4.5f;
			Bullet->GetComponent<ParticleSystem>()->shape->maxYSize = 4.5f;

			Bullet->AddComponent<MachineGunBullet>();
			Bullet->GetComponent<MachineGunBullet>()->OnStart();

			Bullet->m_transform->SetScale(glm::vec3(10, 10, 1));

			Bullet->SetActive(false);
			BulletPool->AddObject(Bullet);
		}
		gamecontroller->GetComponent<GameController>()->AddPool(BulletPool, POOL_TYPE::BULLET_MG);

		//grenade bullet
		ObjectPool* GLbulletpool = new ObjectPool();
		for (int i = 0; i < 20; i++)
		{
			GameObject* Bullet = Instantiate().get();
			Bullet->Layer = "Bullet";
			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(2, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/grenadeL_bullet.png");

			Bullet->AddComponent<Rigidbody>();
			Bullet->GetComponent<Rigidbody>()->Init(7, 7);
			Bullet->GetComponent<Rigidbody>()->SetGravityScale(1.0f);

			Bullet->m_transform->SetScale(glm::vec3(30, 30, 1));

			Bullet->AddComponent<GrenadeLauncherBullet>();
			Bullet->GetComponent<GrenadeLauncherBullet>()->OnStart();

			Bullet->SetActive(false);
			GLbulletpool->AddObject(Bullet);
		}
		gamecontroller->GetComponent<GameController>()->AddPool(GLbulletpool, POOL_TYPE::BULLET_GL);

		//zapper bullet
		ObjectPool* Zapbulletpool = new ObjectPool();
		for (int i = 0; i < 20; i++)
		{
			GameObject* Bullet = Instantiate().get();
			Bullet->Layer = "Bullet";
			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blue.jpg");

			Bullet->AddComponent<Rigidbody>();
			Bullet->GetComponent<Rigidbody>()->Init(7, 7);
			Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0f);

			Bullet->m_transform->SetScale(glm::vec3(10, 10, 1));

			Bullet->AddComponent<ZapperGunBullet>();
			Bullet->GetComponent<ZapperGunBullet>()->OnStart();

			Bullet->SetActive(false);
			Zapbulletpool->AddObject(Bullet);
		}
		gamecontroller->GetComponent<GameController>()->AddPool(Zapbulletpool, POOL_TYPE::BULLET_ZP);

		//blackhole bullet
		ObjectPool* BHbulletpool = new ObjectPool();
		for (int i = 0; i < 20; i++)
		{
			GameObject* Bullet = Instantiate().get();
			Bullet->Layer = "Bullet";
			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blackhole_bullet.png");

			Bullet->AddComponent<Rigidbody>();
			Bullet->GetComponent<Rigidbody>()->Init(7, 7);
			Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

			Bullet->AddComponent<ParticleSystem>();
			Bullet->GetComponent<ParticleSystem>()->emitter->isEnabled = true;

			Bullet->m_transform->SetScale(glm::vec3(30, 30, 1));

			Bullet->AddComponent<BlackholeGunBullet>();
			Bullet->GetComponent<BlackholeGunBullet>()->OnStart();

			Bullet->SetActive(false);
			BHbulletpool->AddObject(Bullet);
		}
		gamecontroller->GetComponent<GameController>()->AddPool(BHbulletpool, POOL_TYPE::BULLET_BH);

		//fume bullet
		ObjectPool* Fumebulletpool = new ObjectPool();
		for (int i = 0; i < 20; i++)
		{
			GameObject* Bullet = Instantiate().get();
			Bullet->Layer = "EnemyBullet";

			Bullet->m_transform->SetScale(glm::vec3(20, 20, 1));


			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/blackhole_bullet.png");
			Bullet->GetComponent<MeshRenderer>()->SetReplaceColor(glm::vec3(50.0 / 255.0, 168.0 / 255.0, 82.0 / 255.0));

			Bullet->AddComponent<Rigidbody>();
			//Bullet->GetComponent<Rigidbody>()->Init(7, 7);
			Bullet->GetComponent<Rigidbody>()->SetGravityScale(0.0000001f);

			Bullet->AddComponent<BoxCollider>()->ReScale(1, 1);
			//add fume bullet component

			Bullet->AddComponent<ParticleSystem>();
			Bullet->GetComponent<ParticleSystem>()->emitter->isEnabled = true;
			Bullet->GetComponent<ParticleSystem>()->emitter->particleSamples = 10;
			Bullet->GetComponent<ParticleSystem>()->emitter->particleRate = 3.0f;

			Bullet->SetActive(false);

			Fumebulletpool->AddObject(Bullet);
		}
		gamecontroller->GetComponent<GameController>()->AddPool(Fumebulletpool, POOL_TYPE::BULLET_FUME);


		//QUEEN ====================================================================================
		std::shared_ptr<Animation> queenIdle = std::make_shared<Animation>();

		queenIdle->setStartPosition(0, 0);
		queenIdle->setEndPosition(3, 0);
		queenIdle->setSpeedMultiplier(1);

		std::shared_ptr<Animation> queenSpawning = std::make_shared<Animation>();

		queenSpawning->setStartPosition(0, 1);
		queenSpawning->setEndPosition(3, 1);
		queenSpawning->setSpeedMultiplier(1);

		queenAnimControl = std::make_shared<AnimationController>();
		queenAnimControl->setSheetSize(glm::vec2(4, 2));
		queenAnimControl->AddState(queenIdle, true);
		queenAnimControl->AddState(queenSpawning, true);


		queen = Instantiate().get();
		queen->Layer = "Enemy";

		queen->m_transform->SetScale(glm::vec3(CHAR_SIZE * 10, CHAR_SIZE * 10, 1.0f));
		queen->m_transform->SetPosition(glm::vec3(-(Graphic::Window::GetWidth()), (Graphic::Window::GetHeight() * 2 / 3) + 700.0f, 1.0f));

		queen->AddComponent<MeshRenderer>();
		queen->GetComponent<MeshRenderer>()->CreateMesh(4, 2);
		queen->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Queen_V[version01].png");

		queen->AddComponent<Animator>();
		queen->GetComponent<Animator>()->AssignController(queenAnimControl);
		queen->GetComponent<Animator>()->setCurrentState(0);
		queen->GetComponent<Animator>()->setFramePerSec(3);

		queen->AddComponent<Rigidbody>();
		queen->GetComponent<Rigidbody>()->Init(400, 300);
		queen->GetComponent<Rigidbody>()->SetGravityScale(0.00001);

		queen->AddComponent<HPsystem>();
		queen->GetComponent<HPsystem>()->SetMaxHP(1000.0f);
		queen->GetComponent<HPsystem>()->SetHp(1000.0f);
		//queen->AddComponent<AirPatrol>();
		//queen->GetComponent<AirPatrol>()->SetPoint(-(Graphic::Window::GetWidth() * 2), Graphic::Window::GetWidth() * 2);
		//queen->AddComponent<DeQueen>();


		//queen->GetComponent<DeQueen>()->assignFlyPool(FlyerPool);
		//queen->GetComponent<DeQueen>()->assignBombPool(BomberPool);



		//Add Sound
		Bg2->AddComponent<SoundPlayer>();
		Bg2->GetComponent<SoundPlayer>()->CreateSoundPlayer();
		Bg2->GetComponent<SoundPlayer>()->SetSound("Sources/Assets/BGMPrototype.mp3");
		Bg2->GetComponent<SoundPlayer>()->SetLoop(true);
		Bg2->GetComponent<SoundPlayer>()->SetVolume(0.5);
		//Bg2->GetComponent<SoundPlayer>()->PlaySound();


		GAME_INFO(*Rabbit);

		//this point is where it all ends =========================================
		
		//Serialization::SaveObject(*(SceneManagement::ActiveScene), "testscene.scene");

		//SceneManagement::LoadScene("testscene.scene");

		//ENGINE_INFO("mesh count {}", Factory<MeshRenderer>::getCollection().size());
		//ENGINE_INFO("GO Active {}", SceneManagement::ActiveScene->GameObjectsInScene.size());
		//for (MeshRenderer* mr : Factory<MeshRenderer>::getCollection()) {
		//	ENGINE_INFO("MR active {}", mr->enabled);
		//}

		SceneManagement::ActiveScene->Init();
	}

	void FixedUpdate(float dt)
	{

		Bg1->m_transform->SetPosition(Graphic::getCamera()->m_position * parlx1);
		Bg2->m_transform->SetPosition(Graphic::getCamera()->m_position * parlx2);

		//Update Physics Scene
		static float accumulator = 0.0f;
		accumulator += dt;

		while (accumulator >= c_targetDT)
		{
			//Update Physic
			g_physicScene->Update(c_targetDT);

			//Update Components
			FactoryCollection::FixedUpdateComponents(c_targetDT);

			accumulator -= c_targetDT;

			g_physicScene->SendCollisionMsg();
		}

	}

	void Update(float dt)
	{
		//Update Sound
		//Bg2->GetComponent<SoundPlayer>()->UpdateVolume();

		//Update All Systems
		//Update Input
		Input::Update();
		//Core
		DebugInput(dt);

		FactoryCollection::UpdateComponents(dt);

		//ENGINE_INFO("FPS: {}", (1/dt));
		//Update Graphic
		Graphic::Render();

	}

	void Loop(void)
	{
		while (!g_gameInfo->m_shouldClose)
		{

			g_gameInfo->StartFrame();

			FixedUpdate(g_gameInfo->m_deltaTime);
			Update(g_gameInfo->m_deltaTime);


			if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE))
			{
				g_gameInfo->GameShouldClose();
				Graphic::Window::SetWindowShouldClose(true);
			}

			g_gameInfo->EndFrame();
		}
	}

	void Terminate(void)
	{
		//Terminate All System
		//Terminate Game

		//Terminate Input
		Input::Terminate();
		//Terminate Graphic
		Graphic::Terminate();
		//Terminate Physics
		delete g_physicScene;
		//Terminate Game Info
		ENGINE_WARN("Engine Terminated");
	}

}
