#include "SceneEditor.hpp"
#include "Core/EC/GameObject.hpp"

namespace Tools
{
	void SceneEditor::Init(void)
	{

	}
	void SceneEditor::Update(void)
	{
	}
	void SceneEditor::Terminate(void)
	{
	}
	bool SceneEditor::Save(const char * path, const char* returnMessage)
	{
		return false;
	}
	bool SceneEditor::Load(const char * path, const char* returnMessage)
	{
		return false;
	}
	bool SceneEditor::LoadScene(const char*)
	{
		return false;
	}
	bool SceneEditor::SaveScene(const char*)
	{
		return false;
	}
	/*GameObject * SceneEditor::GetSelectedGameObject(void)
	{
		return m_selectedGameObject;
	}*/
}

