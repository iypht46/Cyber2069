#include "ACEditor.hpp"

namespace Tools
{
	ACEditor::ACEditor(bool* isOpen) : Editor("AnimationController", isOpen)
	{
		this->Init();
	}

	ACEditor::ACEditor() : Editor("AnimationController")
	{
		this->Init();
	}

	void ACEditor::Init(void)
	{
		//m_entityACTester = make_unique<EditorEntity>();
	}

	void ACEditor::Terminate(void)
	{
	}

	void ACEditor::Update(void)
	{

	}

	bool ACEditor::Save(Utility::fs::path path, Container::wString& returnMessage)
	{
		return false;
	}

	bool ACEditor::Load(Utility::fs::path path, Container::wString& returnMessage)
	{
		return false;
	}

	bool ACEditor::New()
	{
		return false;
	}
}