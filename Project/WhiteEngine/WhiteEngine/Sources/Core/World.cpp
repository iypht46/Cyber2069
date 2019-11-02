//System Headers
#include "Core/World.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Graphic/Camera.hpp"
#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "EC/Components/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"
#include "EC/Components/FlyerBehaviour.hpp"

#include "Factory.h"
#include "Core/EC/GameObject.hpp"

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


	//====================================
	//TESTING ONLY, DON'T FORGET TO REMOVE
	//====================================
	GameObject* Rabbit;
	GameObject* Bg;
	GameObject* Child;
	GameObject* Flyer;

	GameObject* Enemy;

	Animation* Fly;
	AnimationController* EnemCon;

	bool running;
	int Delay = 0;
	int Delay2 = 0;
	int enemyNum;
	int rand_AD;
	int rand_WS;
#define MOVE_SPEED 300.0f
#define MOVE_SPEED_ENEM 100.0f
#define CHAR_SIZE 120.0f
#define ENEMY_SIZE 40.0f

	//====================================

	//Physic Scene
	//static PhysicScene* g_physicScene;

	void DebugInput(float dt)
	{

		if (Input::GetKeyHold(Input::KeyCode::KEY_W))
		{
			Rabbit->m_transform.Translate(glm::vec3(0.0f, MOVE_SPEED * dt, 0.0f));
			cam->Translate(glm::vec3(0.0f, MOVE_SPEED * dt, 0.0f));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_A))
		{
			Rabbit->m_transform.Translate(glm::vec3(-MOVE_SPEED * dt, 0.0f, 0.0f));
			cam->Translate(glm::vec3(-MOVE_SPEED * dt, 0.0f, 0.0f));
			Rabbit->m_transform.SetScale(glm::vec3(-CHAR_SIZE, CHAR_SIZE, 1));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_S))
		{
			Rabbit->m_transform.Translate(glm::vec3(0.0f, -MOVE_SPEED * dt, 0.0f));
			cam->Translate(glm::vec3(0.0f, -MOVE_SPEED * dt, 0.0f));
		}

		if (Input::GetKeyHold(Input::KeyCode::KEY_D))
		{
			Rabbit->m_transform.Translate(glm::vec3(MOVE_SPEED * dt, 0.0f, 0.0f));
			cam->Translate(glm::vec3(MOVE_SPEED * dt, 0.0f, 0.0f));
			Rabbit->m_transform.SetScale(glm::vec3(CHAR_SIZE, CHAR_SIZE, 1));
		}

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

		if (!Input::GetKeyHold(Input::KeyCode::KEY_A) && !Input::GetKeyHold(Input::KeyCode::KEY_D))
		{
			if (running) {
				running = false;
				Rabbit->GetComponent<Animator>()->setCurrentState(0);
			}
		}
		else {

			if (!running) {
				running = true;
				Rabbit->GetComponent<Animator>()->setCurrentState(1);
			}
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
			Rabbit->m_transform.Rotate(1.0f);
		}
	}

	void Init(void)
	{
		srand(time(NULL));
		g_gameInfo = &(GameInfo::GetInstance());
		g_isDebug = false;
		//Initialize All System
		//Physics
		//g_physicScene = new PhysicScene();
		//Core

		//Runtime
		Core::Logger::Init();
		Graphic::Init();

		//Input
		//Bool for debugging
		Input::Init(false);

		ENGINE_WARN("Engine Initialized");

		//GameObject
		Rabbit = new GameObject();
		Bg = new GameObject();
		Child = new GameObject();
		Flyer = new GameObject();

		//Add Renderer

		Bg->AddComponent<MeshRenderer>();
		Bg->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg->GetComponent<MeshRenderer>()->SetTexture("Sources/mockup_BG.png");

		Bg->m_transform.SetScale(glm::vec3(Graphic::Window::GetWidth(), Graphic::Window::GetHeight(), 1));

		Rabbit->AddComponent<MeshRenderer>();
		Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
		Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_PlayerBody_Vversion03.png");

		Child->AddComponent<MeshRenderer>();
		Child->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
		Child->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_PlayerBody_Vversion02.png");

		Child->m_transform.SetParent(&Rabbit->m_transform);

		Flyer->AddComponent<MeshRenderer>();
		Flyer->GetComponent<MeshRenderer>()->CreateMesh(5, 1);
		Flyer->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_Enemy_Flyer_Vversion01.png");

		//Add Animator
		Animation* Idle = new Animation();

		Idle->setStartPosition(0, 0);
		Idle->setEndPosition(6, 0);

		Animation* Running = new Animation();

		Running->setStartPosition(0, 1);
		Running->setEndPosition(4, 1);

		AnimationController* RabbitController = new AnimationController();
		RabbitController->setSheetSize(glm::vec2(7, 5));

		RabbitController->AddState(Idle);
		RabbitController->AddState(Running);

		Rabbit->AddComponent<Animator>();
		Rabbit->GetComponent<Animator>()->AssignController(RabbitController);
		Rabbit->GetComponent<Animator>()->setCurrentState(0);

		Rabbit->m_transform.SetScale(glm::vec3(500, 500, 1));

		Child->m_transform.SetScale(glm::vec3(100, 100, 1));
		Child->m_transform.SetPosition(glm::vec3(100, 100, 0));
		Child->m_transform.SetLocalPosition(glm::vec3(100, 100, 0));
		Bg->m_transform.SetScale(glm::vec3(500, 500, 1));

		Rabbit->m_transform.SetScale(glm::vec3(CHAR_SIZE, CHAR_SIZE, 1));

		Flyer->m_transform.SetPosition(glm::vec3(100, 100, 0));
		Flyer->m_transform.SetScale(glm::vec3(50, 50, 1));
		
		Fly = new Animation();
		Fly->setStartPosition(0, 0);
		Fly->setEndPosition(5, 0);

		EnemCon = new AnimationController();

		EnemCon->setSheetSize(glm::vec2(6, 1));
		EnemCon->AddState(Fly);

		Flyer->AddComponent<Animator>();
		Flyer->GetComponent<Animator>()->AssignController(EnemCon);
		Flyer->GetComponent<Animator>()->setCurrentState(0);

		Flyer->AddComponent<FlyerBehaviour>();
		Flyer->GetComponent<FlyerBehaviour>()->SetPlayer((Rabbit->m_transform));
		Flyer->GetComponent<FlyerBehaviour>()->SetGameObject(Flyer);
	}

	void FixedUpdate(float dt)
	{
		//Update Physics Scene
		static float accumulator = 0.0f;
		accumulator += dt;

		while (accumulator >= c_targetDT)
		{
			//Update Physic
			//g_physicScene->Update(c_targetDT);
			//ENGINE_INFO("FixedUpdate: {}", dt);
			accumulator -= c_targetDT;
		}

		/*Factory<GameObject>::Create();
		std::vector<GameObject*> a = Factory<GameObject>::getCollection();
		a.back()->AddComponent<MeshRenderer>();
		std::cout << Factory<MeshRenderer>::getCollection().size() << std::endl;*/
	}


	void Update(float dt)
	{
		//Update All Systems
		//Update Input
		Input::Update();
		//Core
		DebugInput(dt);

		Flyer->GetComponent<FlyerBehaviour>()->OnUpdate(dt);

		for (int i = 0; i < Factory<Animator>::getCollection().size(); i++)
		{
			Factory<Animator>::getCollection().at(i)->animUpdate();
		}

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
		//delete g_physicScene;
		//Terminate Game Info
		ENGINE_WARN("Engine Terminated");
	}

}
