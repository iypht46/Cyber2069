#pragma once

namespace Tools
{
	class Editor
	{
	protected:
		bool m_hasSaved;
	public:
		bool HasSaved();
		virtual void Init(void) = 0;
		virtual void Update(void) = 0;
		virtual void Terminate(void) = 0;
		virtual bool Save(const char*) = 0;
		virtual bool Load(const char*) = 0;
	};

	inline bool Editor::HasSaved() { return m_hasSaved; }

	
}
