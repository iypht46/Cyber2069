#include "Core/GameInfo.h"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"
#include "Graphic/Window.hpp"
#include "SDL.h"

namespace World
{
	using Clock = std::chrono::high_resolution_clock;

	GameInfo& GameInfo::GetInstance(void)
	{
		static GameInfo info;
		return info;
	}

	void GameInfo::StartFrame(void)
	{
		m_frameStartTime = Clock::now();
	}

	void GameInfo::EndFrame(void)
	{
		//Check if the time taken in this frame is less that the expected time for each frame. (Time in this context is ms per frame)
		FloatDuration delayTime = m_expectedDeltaTime - (Clock::now() - m_frameStartTime);

		//If the delay time is negative then the frame took longer that it should
		//but if not then we delay until this frame time reach the expected time.
		if (delayTime.count() > 0.0f)
		{
			
			SDL_Delay(static_cast<Uint32>(delayTime.count()));
			
		}

		//Assign the delta time for this frame
		m_deltaTime = FloatDuration(Clock::now() - m_frameStartTime).count();
		//Set the framerate
		m_frameRate = 1.0f / m_deltaTime;
		
	}

	void GameInfo::GameShouldClose()
	{
			m_shouldClose = true;
	}

	void GameInfo::HandleMessage(const Core::GameCloseMessage& msg)
	{
		GameShouldClose();
	}
}