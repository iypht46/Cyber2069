//System Headers
#include "World.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Graphic/Camera.hpp"
#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "Core/EC/Components/Animator.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/Components/SoundPlayer.hpp"

#include "FlyerBehaviour.hpp"
#include "PlayerController.hpp"
#include "MachineGunBullet.hpp"
#include "EnemySpawner.hpp"

#include "Core/EC/Components/Collider.hpp"
#include "Core/EC/Components/Rigidbody.hpp"

#include "Core/Factory.h"
#include "FactoryCollection.h"
#include "Core/EC/GameObject.hpp"

#include "Utility/ObjectPool.h"

#include "Physic/PhysicScene.hpp"
#include "Physic/Collision.hpp"

#include <stdlib.h>
#include <time.h>

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
	ObjectPool* FlyerPool;

	GameObject* title;
	GameObject* Rabbit;
	GameObject* Bg1;
	GameObject* Bg2;
	GameObject* Child;
	GameObject* Flyer;
	GameObject** platform;

	GameObject* Enemy;
	GameObject* Spawner;

	Animation* Fly;
	AnimationController* EnemCon;

	GameObject** test;

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

		if (Input::GetKeyHold(Input::KeyCode::KEY_R))
		{
			cam->ResetCam();
			Rabbit->m_transform.SetPosition(glm::vec3(0.0f, 0.0f, 1));
		}

		//child
		if (Input::GetKeyHold(Input::KeyCode::KEY_H))
		{
			Child->m_transform.SetLocalPosition(Child->m_transform.GetLocalPosition() + glm::vec3(MOVE_SPEED * dt, 0.0f, 0.0f));
		}
		if (Input::GetKeyHold(Input::KeyCode::KEY_F))
		{
			Child->m_transform.SetLocalPosition(Child->m_transform.GetLocalPosition() + glm::vec3(-MOVE_SPEED * dt, 0.0f, 0.0f));
		}
		if (Input::GetKeyHold(Input::KeyCode::KEY_T))
		{
			Child->m_transform.SetLocalPosition(Child->m_transform.GetLocalPosition() + glm::vec3(0.0f, MOVE_SPEED * dt, 0.0f));
		}
		if (Input::GetKeyHold(Input::KeyCode::KEY_G))
		{
			Child->m_transform.SetLocalPosition(Child->m_transform.GetLocalPosition() + glm::vec3(0.0f, -MOVE_SPEED * dt, 0.0f));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Q))
		{
			Rabbit->m_transform.Rotate(1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_E))
		{
			Rabbit->m_transform.Rotate(-1.0f);
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_Z))
		{
			Rabbit->m_transform.SetScale(Rabbit->m_transform.GetScale() + glm::vec3(1, 0, 0));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_C))
		{
			Rabbit->m_transform.SetScale(Rabbit->m_transform.GetScale() + glm::vec3(-1, 0, 0));

		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_N))
		{
			Bg2->GetComponent<SoundPlayer>()->IncreaseVolume();
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_M))
		{
			Bg2->GetComponent<SoundPlayer>()->DecreaseVolume();
		}
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

		//std::cout << "Layer Collision: " << g_physicScene->GetLayerCollisions("Player") << std::endl;

		ENGINE_WARN("Engine Initialized");
		//ENGINE_INFO("Layer Static Cast: {}", static_cast<uint32_t>(Physic::Layer::PHYSIC_LAYER_1));
		//ENGINE_INFO("Layer Static Cast: {}", static_cast<uint32_t>(Physic::Layer::PHYSIC_LAYER_2));

		//GameObject
		enemyNum = 10;
		title = new GameObject();
		test = new GameObject*[enemyNum];
		Rabbit = new GameObject();
		Bg2 = new GameObject();
		Bg1 = new GameObject();
		Child = new GameObject();
		Flyer = new GameObject();
		platform = new GameObject*[platformNum];

		Spawner = new GameObject();

		BulletPool = new ObjectPool();
		FlyerPool = new ObjectPool();

		//Add Renderer

		title->AddComponent<MeshRenderer>();
		title->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		title->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/mockup_title.jpg");

		title->m_transform.SetScale(glm::vec3(Graphic::Window::GetWidth(), Graphic::Window::GetHeight(), 1.0f));

		while (!Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
		{
			Input::Update();

			Graphic::Render();

			if (Input::GetKeyDown(Input::KeyCode::KEY_SPACE))
			{
				title->SetActive(false);
			}
		}

		Bg2->AddComponent<MeshRenderer>();
		Bg2->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg2->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer2.png");

		Bg1->AddComponent<MeshRenderer>();
		Bg1->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg1->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Background_Layer1.png");

		Bg2->m_transform.SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));
		Bg1->m_transform.SetScale(glm::vec3(Graphic::Window::GetWidth() * 2.0f, Graphic::Window::GetHeight() * 2.0f, 1));

		Bg1->m_transform.SetPosition(glm::vec3(0, -300, 0));
		Bg2->m_transform.SetPosition(glm::vec3(0, -300, 0));

		Rabbit->AddComponent<MeshRenderer>();
		Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
		Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_PlayerBody_Vversion03.png");

		Child->m_transform.SetParent(&Rabbit->m_transform);

		Flyer->AddComponent<MeshRenderer>();
		Flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
		Flyer->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Flyer_Vversion01.png");



		//Add Animator
		Animation* Idle = new Animation();

		Idle->setStartPosition(0, 0);
		Idle->setEndPosition(6, 0);
		Idle->setSpeedMultiplier(1);

		Animation* Running = new Animation();

		Running->setStartPosition(0, 1);
		Running->setEndPosition(4, 1);
		Running->setSpeedMultiplier(2);

		Animation* Dashing = new Animation();

		Dashing->setStartPosition(0, 4);
		Dashing->setEndPosition(2, 4);
		Dashing->setSpeedMultiplier(1);

		Animation* Jumping = new Animation();

		Jumping->setStartPosition(0, 3);
		Jumping->setEndPosition(3, 3);
		Jumping->setSpeedMultiplier(1);

		Animation* falling = new Animation();
		falling->setStartPosition(3, 3);
		falling->setEndPosition(3, 3);
		falling->setSpeedMultiplier(1);


		Idle->setLooping(true);
		Running->setLooping(true);
		Dashing->setLooping(false);
		Jumping->setLooping(false);
		falling->setLooping(false);

		AnimationController* RabbitController = new AnimationController();
		RabbitController->setSheetSize(glm::vec2(7, 5));

		RabbitController->AddState(Idle);
		RabbitController->AddState(Running);
		RabbitController->AddState(Dashing);
		RabbitController->AddState(Jumping);
		RabbitController->AddState(falling);

		Rabbit->AddComponent<Animator>();
		Rabbit->GetComponent<Animator>()->AssignController(RabbitController);
		Rabbit->GetComponent<Animator>()->setCurrentState(0);
		Rabbit->GetComponent<Animator>()->setFramePerSec(12);

		Fly = new Animation();
		Fly->setStartPosition(0, 0);
		Fly->setEndPosition(5, 0);
		Fly->setSpeedMultiplier(3);
		Fly->setLooping(true);

		EnemCon = new AnimationController();

		EnemCon->setSheetSize(glm::vec2(6, 1));
		EnemCon->AddState(Fly);

		Flyer->AddComponent<Animator>();
		Flyer->GetComponent<Animator>()->AssignController(EnemCon);
		Flyer->GetComponent<Animator>()->setCurrentState(0);
		Flyer->GetComponent<Animator>()->setFramePerSec(12);

		for (int i = 0; i < 100; i++)
		{
			GameObject* Bullet = new GameObject();
			Bullet->AddComponent<MeshRenderer>();
			Bullet->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
			Bullet->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_bullet.png");

			Bullet->AddComponent<Rigidbody>();
			Bullet->GetComponent<Rigidbody>()->Init();

			Bullet->AddComponent<MachineGunBullet>();
			Bullet->GetComponent<MachineGunBullet>()->OnStart();

			Bullet->m_transform.SetScale(glm::vec3(10, 10, 1));

			Bullet->SetActive(false);
			BulletPool->AddObject(Bullet);
		}

		Child->AddComponent<MeshRenderer>();
		Child->GetComponent<MeshRenderer>()->CreateMesh(4, 1);
		Child->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/machinegun_shoot.png");
		//std::cout << "Layer Collision: " << g_physicScene->GetLayerCollisions("Player") << std::endl;


		//Set Transform

		//Rabbit->m_transform.SetScale(glm::vec3(500, 500, 1));

		Rabbit->m_transform.SetScale(glm::vec3(CHAR_SIZE, CHAR_SIZE, 1));
		Rabbit->m_transform.SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));

		Child->m_transform.SetScale(glm::vec3(70, 70, 1));
		//Child->m_transform.SetLocalScale(glm::vec3(1, 1, 1));
		//Child->m_transform.SetPosition(glm::vec3(0, 0, 0));
		Child->m_transform.SetLocalPosition(glm::vec3(1, 0, 0));
		//Bg->m_transform.SetScale(glm::vec3(500, 500, 1));

		Flyer->m_transform.SetPosition(glm::vec3(100, 100, 0));
		Flyer->m_transform.SetScale(glm::vec3(50, 50, 1));

		//Add Physic
		g_physicScene->SetLayerName("Player", Physic::Layer::PHYSIC_LAYER_1);
		g_physicScene->SetLayerName("Enemy", Physic::Layer::PHYSIC_LAYER_2);
		g_physicScene->SetLayerName("Platform", Physic::Layer::PHYSIC_LAYER_3);
		//g_physicScene->SetLayerCollisions("Player", "Enemy");
		g_physicScene->SetLayerCollisions("Player", "Platform");
		Rabbit->AddComponent<Rigidbody>()->Init(20, 20);
		Rabbit->GetComponent<Rigidbody>()->SetDrag(0.01f);

		Flyer->AddComponent<Rigidbody>()->Init(10,10);
		Flyer->GetComponent<BoxCollider>()->SetTrigger(true);
		g_physicScene->Add(Rabbit->GetComponent<BoxCollider>(), "Player");
		g_physicScene->Add(Flyer->GetComponent<BoxCollider>(), "Enemy");
		g_physicScene->Add(Rabbit->GetComponent<Rigidbody>());
		g_physicScene->Add(Flyer->GetComponent<Rigidbody>());

		//Behavior Script
		Rabbit->AddComponent<PlayerController>();
		Rabbit->GetComponent<PlayerController>()->OnStart();
		Rabbit->GetComponent<PlayerController>()->assignPool(BulletPool);

		Flyer->AddComponent<FlyerBehaviour>();
		Flyer->GetComponent<FlyerBehaviour>()->SetPlayer((Rabbit->m_transform));
		Flyer->GetComponent<FlyerBehaviour>()->SetGameObject(Flyer);

		for (int i = 0; i < 100; i++)
		{
			GameObject* flyer = new GameObject();
			flyer->AddComponent<MeshRenderer>();
			flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
			flyer->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/Mockup_Enemy_Flyer_Vversion01.png");

			flyer->AddComponent<Rigidbody>();
			flyer->GetComponent<Rigidbody>()->Init();

			g_physicScene->Add(flyer->GetComponent<Rigidbody>());

			flyer->AddComponent<FlyerBehaviour>();
			flyer->GetComponent<FlyerBehaviour>()->SetPlayer((Rabbit->m_transform));
			flyer->GetComponent<FlyerBehaviour>()->SetGameObject(flyer);

			flyer->AddComponent<Animator>();
			flyer->GetComponent<Animator>()->AssignController(EnemCon);
			flyer->GetComponent<Animator>()->setCurrentState(0);
			flyer->GetComponent<Animator>()->setFramePerSec(12);

			flyer->m_transform.SetScale(glm::vec3(50, 50, 1));

			flyer->SetActive(false);
			FlyerPool->AddObject(flyer);
		}

		Spawner->AddComponent<EnemySpawner>();
		Spawner->GetComponent<EnemySpawner>()->OnStart();
		Spawner->GetComponent<EnemySpawner>()->assignPool(FlyerPool);

		for (int i = 0; i < platformNum; i++)
		{
			platform[i] = new GameObject();
			platform[i]->AddComponent<MeshRenderer>();
			platform[i]->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
			platform[i]->GetComponent<MeshRenderer>()->SetTexture("Sources/Assets/platform01.png");
			platform[i]->m_transform.SetScale(glm::vec3(400, 20, 1));
			platform[i]->AddComponent<BoxCollider>()->Init(180, 5);
			g_physicScene->Add(platform[i]->GetComponent<BoxCollider>(), "Platform");
		}

		platform[1]->m_transform.SetPosition(glm::vec3(300, 300, 0));
		platform[2]->m_transform.SetPosition(glm::vec3(-300, 300, 0));
		platform[3]->m_transform.SetPosition(glm::vec3(-300, -300, 0));
		platform[4]->m_transform.SetPosition(glm::vec3(300, -300, 0));

		//g_physicScene->Add(Rabbit->AddComponent<Rigidbody>());
		//Rabbit->GetComponent<Rigidbody>()->Init(0.83, 0.25f);

		/*for (int i = 0; i < enemyNum; i++)
		{
			float randX = (float)((rand() % 350) - (rand() % 350));
			float randY = (float)((rand() % 350) - (rand() % 350));
			test[i] = new GameObject();
			test[i]->AddComponent<MeshRenderer>();
			test[i]->GetComponent<MeshRenderer>()->CreateMesh(6, 1);
			test[i]->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_Enemy_Flyer_Vversion01.png");
			test[i]->m_transform.SetScale(glm::vec3(40.0f, 30.0f, 1));
			test[i]->m_transform.SetPosition(glm::vec3(randX, randY, 0.0f));
			Rigidbody* testRigid = test[i]->AddComponent<Rigidbody>();
			testRigid->Init(5, 10);
			g_physicScene->Add(testRigid->GetCollider(), "Enemy");

		}*/

		//Add Sound
		Bg2->AddComponent<SoundPlayer>();
		Bg2->GetComponent<SoundPlayer>()->CreateSoundPlayer();
		Bg2->GetComponent<SoundPlayer>()->SetSound("Sources/Assets/BGMPrototype.mp3");
		Bg2->GetComponent<SoundPlayer>()->SetLoop(true);
		Bg2->GetComponent<SoundPlayer>()->SetVolume(0.5);
		//Bg2->GetComponent<SoundPlayer>()->PlaySound();
		//Bg->GetComponent<SoundPlayer>()->DeleteSoundPlayer();

	}

	void FixedUpdate(float dt)
	{

		Bg1->m_transform.SetPosition(Graphic::getCamera()->m_position * parlx1);
		Bg2->m_transform.SetPosition(Graphic::getCamera()->m_position * parlx2);

		//Update Physics Scene
		static float accumulator = 0.0f;
		accumulator += dt;

		while (accumulator >= c_targetDT)
		{
			//Update Physic

			FactoryCollection::FixedUpdateComponents(dt);

			/*Rabbit->GetComponent<Rigidbody>()->AddForce(glm::vec3(0.0f, -10.0f, 0.0f));
			Rabbit->GetComponent<Rigidbody>()->UpdateTransform(dt);*/
			//Flyer->GetComponent<Rigidbody>()->UpdateTransform(dt);

			for (int i = 0; i < Factory<MachineGunBullet>::getCollection().size(); i++)
			{
				if (Factory<MachineGunBullet>::getCollection().at(i)->GetGameObject()->Active())
				{
					Factory<MachineGunBullet>::getCollection().at(i)->GetGameObject()->GetComponent<Rigidbody>()->UpdateTransform(dt);
				}
			}

			g_physicScene->Update(c_targetDT);
			//ENGINE_INFO("FixedUpdate: {}", dt
			/*Physic::Manifold col(Rabbit->GetComponent<Collider>(), test->GetComponent<Collider>());
			if (col.CheckCollision())
			{
				ENGINE_INFO("Collided");
			}*/
			//ENGINE_INFO("Velocity: {}, {}", Rabbit->GetComponent<Rigidbody>()->GetVelocity().x ,Rabbit->GetComponent<Rigidbody>()->GetVelocity().y);
			accumulator -= c_targetDT;
		}

		g_physicScene->SendCollisionMsg();
		//ENGINE_INFO()
		//cout << Rabbit->m_transform.GetPosition().y << endl;

		/*Factory<GameObject>::Create();
		std::vector<GameObject*> a = Factory<GameObject>::getCollection();
		a.back()->AddComponent<MeshRenderer>();
		std::cout << Factory<MeshRenderer>::getCollection().size() << std::endl;*/
	}

	void Update(float dt)
	{
		//update sound
		Bg2->GetComponent<SoundPlayer>()->UpdateVolume();

		//Update All Systems
		//Update Input
		Input::Update();
		//Core
		DebugInput(dt);

		//Flyer->GetComponent<FlyerBehaviour>()->OnUpdate(dt);
		FactoryCollection::UpdateComponents(dt);

		//for (int i = 0; i < Factory<Animator>::getCollection().size(); i++)
		//{
		//	Factory<Animator>::getCollection().at(i)->animUpdate();
		//}
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
