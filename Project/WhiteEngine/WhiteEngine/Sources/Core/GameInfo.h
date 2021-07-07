#pragma once

#include <chrono>
#include "Input/Input.hpp"
#include "Graphic/Window.hpp"
#include "Core/Message/IMessageHandler.hpp"

namespace World
{
	constexpr float DEFAULT_FRAMERATE = 60.0f;
	struct GameInfo : Core::IMessageHandler
	{
	public:
		//Methods
		void StartFrame(void);
		void EndFrame(void);
		static GameInfo& GetInstance(void);

		//Members
		float m_deltaTime = 1 / DEFAULT_FRAMERATE;
		bool m_shouldClose = false;
		float m_frameRate = 1.0f / m_deltaTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_frameStartTime;

		
		void GameShouldClose(void);

		//IMessage
		virtual void HandleMessage(const Core::GameCloseMessage&);
	};

	
}
