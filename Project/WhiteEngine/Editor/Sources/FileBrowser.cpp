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
		if (m_fileSystem.get() == nullptr)
			m_fileSystem = std::make_shared<fs::path>(path);
		else
		{
			m_fileSystem.reset();
			m_fileSystem = std::make_shared<fs::path>(path);
		}

		if (fs::exists(*FileBrowser::Get()) && fs::is_directory(*FileBrowser::Get()))
			return true;
		else
			return false;
	}
	bool FileBrowser::GetFileInDir(const char *)
	{
		return false;
	}
	bool FileBrowser::GetFilesInDir()
	{
		return false;
	}
}

