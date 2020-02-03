#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <memory>



namespace Tools
{
	namespace fs = std::experimental::filesystem;
	
	class FileBrowser
	{
	private:
		std::unique_ptr<fs::path> m_fileSystem;
		fs::path* Get();
	public:
		//Default Constructor
		FileBrowser();
		static FileBrowser& GetInstance(void);

		//Interface
		//Set file path
		bool OpenFileDir(std::string);
		//Get file

		//Get file lists

	};
}
