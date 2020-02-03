#pragma once

namespace Tools
{
	class UIWindow
	{
	private:
		static bool m_show;
	public:
		virtual void Init(void) = 0;
		virtual void Draw(void) = 0;
		virtual void Terminate(void) = 0;
		bool GetBool();
	};

	inline bool UIWindow::GetBool(void) { return m_show; }
}
