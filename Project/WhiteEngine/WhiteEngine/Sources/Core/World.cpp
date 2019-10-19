//System Headers
#include "Core/World.hpp"

#include "Graphic/GraphicCore.hpp"
#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "Core/Animator/Animator.hpp"
#include "EC/Components/MeshRenderer.hpp"

#include "Factory.h"
#include "Core/EC/GameObject.hpp"

namespace World
{
	//Constants
	constexpr float c_maxFrameRate = 60.0f;
	constexpr float c_targetDT = 1.0f / c_maxFrameRate;

	//Game Info Var
	static GameInfo* g_gameInfo;

	//====================================
	//TESTING ONLY, DON'T FORGET TO REMOVE
	//====================================
	GameObject* Rabbit;
	GameObject* Bg;


	//====================================

	//Physic Scene
	//static PhysicScene* g_physicScene;

	void DebugInput()
	{
		if (Input::GetKeyHold(Input::KeyCode::KEY_W))
		{
			Rabbit->m_transform.Translate(glm::vec3(0.0f, 10.0f, 0.0f));
		}
		
		if (Input::GetKeyHold(Input::KeyCode::KEY_A))
		{
			Rabbit->m_transform.Translate(glm::vec3(-10.0f, 0.0f, 0.0f));
		}
		
		if (Input::GetKeyHold(Input::KeyCode::KEY_S))
		{
			Rabbit->m_transform.Translate(glm::vec3(0.0f, -10.0f, 0.0f));
		}
		
		if (Input::GetKeyHold(Input::KeyCode::KEY_D))
		{
			Rabbit->m_transform.Translate(glm::vec3(10.0f, 0.0f, 0.0f));
		}
	}

	void Init(void)
	{
		g_gameInfo = &(GameInfo::GetInstance());

		//Initialize All System
		//Physics
		//g_physicScene = new PhysicScene();
		//Core

		//Runtime
		Core::Logger::Init();
		Graphic::Init();

		//Input
		Input::Init();

		ENGINE_WARN("Engine Initialized");

		//GameObject
		Rabbit = new GameObject();
		Bg = new GameObject();

		//Add Renderer
		
		/*Bg->AddComponent<MeshRenderer>();
		Bg->GetComponent<MeshRenderer>()->CreateMesh(1, 1);
		Bg->GetComponent<MeshRenderer>()->SetTexture("Sources/mockup_BG.png");*/

		Rabbit->AddComponent<MeshRenderer>();
		Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);
		Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_PlayerBody_Vversion02.png");

		//Add Animator
		Animation* Running = new Animation();

		Running->setStartPosition(0, 1);
		Running->setEndPosition(4, 1);

		AnimationController* RabbitController = new AnimationController();
		RabbitController->setSheetSize(glm::vec2(7, 5));
		RabbitController->AddState(Running);

		Rabbit->AddComponent<Animator>();
		Rabbit->GetComponent<Animator>()->AssignController(RabbitController);
		Rabbit->GetComponent<Animator>()->setCurrentState(0);

		Rabbit->m_transform.SetScale(glm::vec3(100, 100, 1));
		
		Bg->m_transform.SetScale(glm::vec3(500, 500, 1));

	}

	void FixedUpdate(float dt)
	{
		//Update Physics Scene
		static float accumulator = 0.0f;
		accumulator += dt;

		while (accumulator > c_targetDT)
		{
			//Update Physic
			//g_physicScene->Update(c_targetDT);

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
		DebugInput();

		Rabbit->GetComponent<Animator>()->animUpdate();

		//Update Graphic
		Graphic::Render();
	}

	void Loop(void)
	{
		while (!g_gameInfo->m_shouldClose)
		{
			FixedUpdate(g_gameInfo->m_deltaTime);
			Update(g_gameInfo->m_deltaTime);


			if (Input::GetKeyDown(Input::KeyCode::KEY_ESCAPE))
			{
				g_gameInfo->GameShouldClose();
			}
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
