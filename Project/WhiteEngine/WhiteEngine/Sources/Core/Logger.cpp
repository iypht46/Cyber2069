#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Core
{
	std::shared_ptr<spdlog::logger> Logger::s_engineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

	void Logger::Init(void)
	{
		//Set log pattern to: [TIME] LoggerName: Message
		spdlog::set_pattern("%^ [%T] %n:%$ %v");

		//Create logger
		s_engineLogger = spdlog::stdout_color_mt("ENGINE");
		s_clientLogger = spdlog::stdout_color_mt("GAME");

		//Set logger level to all
		s_engineLogger->set_level(spdlog::level::level_enum::trace);
		s_clientLogger->set_level(spdlog::level::level_enum::trace);

		ENGINE_INFO("Logger System Initialized");
	}

}



