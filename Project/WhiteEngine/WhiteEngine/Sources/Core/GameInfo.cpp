#include "Core/GameInfo.h"

namespace World
{
	GameInfo& GameInfo::GetInstance(void)
	{
		static GameInfo info;
		return info;
	}

	void GameInfo::StartFrame(void)
	{
		m_frameStartTime = std::chrono::high_resolution_clock::now();
	}

	void GameInfo::EndFrame(void)
	{
		m_deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_frameStartTime).count();
		m_frameRate = 1.0f / m_deltaTime;
	}

	void GameInfo::GameShouldClose()
	{
			m_shouldClose = true;
	}
}