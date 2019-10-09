//System Headers
#include "Core/World.hpp"
#include "Graphic/GraphicCore.hpp"
#include "Core/GameInfo.h"
#include "Input/Input.hpp"

namespace World
{
	//Constants
	constexpr float c_maxFrameRate = 60.0f;
	constexpr float c_targetDT = 1.0f / c_maxFrameRate;

	//Game Info Var
	static GameInfo* g_gameInfo;

	//Physic Scene
	//static PhysicScene* g_physicScene;

	void Init(void)
	{
		g_gameInfo = &(GameInfo::GetInstance());

		//Initialize All System
		//Physics
		//g_physicScene = new PhysicScene();
		//Core

		//Renderer and Windows
		Graphic::Init();
		//Input
		Input::Init();

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
	}

	void Update(float dt)
	{
		//Update All Systems
		//Update Input
		Input::Update();

		//Update Graphic
		Graphic::Render();
	}

	void Loop(void)
	{
		while (!g_gameInfo->m_shouldClose)
		{
			FixedUpdate(g_gameInfo->m_deltaTime);
			Update(g_gameInfo->m_deltaTime);
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
		delete g_gameInfo;
	}

}

