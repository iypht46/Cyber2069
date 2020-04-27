#pragma once
#include "Editor/Editor.hpp"
#include "EditorObject/EditorEntity.hpp"
#include "Core/Handle.hpp"

namespace Tools
{
	//using EntityHandle = UNIQUE_HANDLE(EditorEntity);

	class ACEditor : public Editor
	{
	public:
		ACEditor(bool* isOpen);
		ACEditor();
		virtual void Update(void) override;
		virtual bool Save(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool Load(Utility::fs::path path, Container::wString& returnMessage) override;
		virtual bool New() override;
	protected:
		virtual void Init(void) override;
		virtual void Terminate(void) override;
	private:
		//TODO: Controller editor window

		//TODO: Entity animation tester
		//EntityHandle m_entityACTester;
		//TODO: Animation Controller Object
		//ACHandle m_acObject;
	};
}



