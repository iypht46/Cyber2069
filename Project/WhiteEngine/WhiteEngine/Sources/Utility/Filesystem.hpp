#pragma once
#include <filesystem>
#include <iostream>
#include "Container/String.hpp"

namespace Utility
{
	namespace fs = std::filesystem;

	namespace File
	{
		static fs::path s_projectPath = fs::current_path();

		inline Container::wString GetRelativePath(Container::wString filePath, Container::wString basePath = Container::wString(std::filesystem::current_path().generic_string()))
		{
			
			std::filesystem::path file(filePath), base(basePath);
			return Container::wString(std::filesystem::relative(file, base).generic_string());
		}

		inline std::filesystem::path GetRelativePath(std::filesystem::path filePath, std::filesystem::path basePath = std::filesystem::current_path())
		{
			return std::filesystem::relative(filePath, basePath);
		}
	}
	
}