#include "FileBrowser.hpp"

namespace Tools
{
	FileBrowser::FileBrowser()
	{

	}

	fs::path* FileBrowser::Get()
	{
		return m_fileSystem.get();
	}

	FileBrowser & FileBrowser::GetInstance(void)
	{
		static FileBrowser fb;
		return fb;
	}

	bool FileBrowser::OpenFileDir(std::string path)
	{
		m_fileSystem = std::make_unique<fs::path>(path);

		if (fs::exists(FileBrowser::Get()))
			return true;
		else
			return false;
		//return false;
	}
}

