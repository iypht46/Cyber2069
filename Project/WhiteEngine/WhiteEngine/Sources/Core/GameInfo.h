#pragma once

#include <chrono>
#include "Core/Message/IMessageHandler.hpp"

namespace World
{
	//Max framerate
	constexpr float c_MAX_FRAMERATE = 60.0f;

	using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using FloatDuration = std::chrono::duration<float>;

	struct GameInfo : Core::IMessageHandler
	{
	public:
		//Methods
		void StartFrame(void);
		void EndFrame(void);
		static GameInfo& GetInstance(void);
		void GameShouldClose(void);

		//Members
		float m_deltaTime = 1 / c_MAX_FRAMERATE;
		float m_frameRate = 1.0f / m_deltaTime;
		float m_maxFramerate = c_MAX_FRAMERATE;
		Time m_frameStartTime;
		FloatDuration m_expectedDeltaTime = FloatDuration(1.0f / m_maxFramerate);
		bool m_shouldClose = false;
		

		//IMessage
		virtual void HandleMessage(const Core::GameCloseMessage&);
	};

	
}
