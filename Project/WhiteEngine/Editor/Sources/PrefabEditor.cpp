#include "PrefabEditor.hpp"
#include "EditorEntity.hpp"
#include "FileBrowser.hpp"

namespace Tools
{
	void PrefabEditor::Init(void)
	{
		m_editorEntitiy = make_unique<EditorEntity>();
		//Initialize GameObject
		//m_editorEntitiy->AddComponent(); //Add Transform
		//m_editorEntitiy->AddComponent(); //Add MeshRenderer
	}

	void PrefabEditor::Update(void)
	{

	}

	void PrefabEditor::Terminate(void)
	{

	}

	bool PrefabEditor::LoadPrefab(const char * filename)
	{
		return false;
	}

	bool PrefabEditor::SavePrefab(const char * filename)
	{
		return false;
	}

	bool PrefabEditor::Save(const char* path)
	{
		
		return false;
	}

	bool PrefabEditor::Load(const char* path)
	{
		std::string pathStr(path);
		if (FileBrowser::GetInstance().OpenFileDir(pathStr))
		{
			if (LoadPrefab(pathStr.c_str()))
			{
				return true;
			}
		}

		return false;
	}
}

