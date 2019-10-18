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


	//====================================

	//Physic Scene
	//static PhysicScene* g_physicScene;

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

		Rabbit->AddComponent<MeshRenderer>();

		
		//dynamic_cast<MeshRenderer*>(Rabbit.GetComponent<MeshRenderer*>())->CreateMesh(7, 5);

		Rabbit->GetComponent<MeshRenderer>()->CreateMesh(7, 5);

		Rabbit->GetComponent<MeshRenderer>()->SetTexture("Sources/Mockup_PlayerBody_Vversion02.png");

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
