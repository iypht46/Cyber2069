#pragma once

//Standard Headers
#include <memory>

//External Headers
#include "spdlog/spdlog.h"

namespace Core
{
	class Logger
	{
	private:
		static std::shared_ptr<spdlog::logger> s_engineLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	public:
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger(void) { return s_engineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger(void) { return s_clientLogger; }

		static void Init(void);
	};
}

//Macros
//Game Logger Macros
#define GAME_TRACE(...) ::Core::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define GAME_INFO(...) ::Core::Logger::GetClientLogger()->info(__VA_ARGS__)
#define GAME_WARN(...) ::Core::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define GAME_ERROR(...) ::Core::Logger::GetClientLogger()->error(__VA_ARGS__)
#define GAME_FATAL(...) ::Core::Logger::GetClientLogger()->fatal(__VA_ARGS__)

//Engine Logger Macros
#define ENGINE_TRACE(...) ::Core::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...) ::Core::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...) ::Core::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...) ::Core::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_FATAL(...) ::Core::Logger::GetEngineLogger()->fatal(__VA_ARGS__)

