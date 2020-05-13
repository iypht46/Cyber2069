#pragma once
#include "Container/String.hpp"
#include <queue>

namespace Tools
{
	using namespace Container;

	struct PopupData
	{
		
		wString m_title;
		wString m_message;
		bool m_isConfirmBox = false;
		PopupData(wString title, wString message, bool isConfirmBox = false) 
			: m_title(title), m_message(message), m_isConfirmBox(isConfirmBox)
		{}
	};

	class PopupWindow
	{
	private:
		const float c_button_x = 80.0f;
		std::queue<PopupData> m_popupQueue;
	protected:
		void DisplayPopup(PopupData& popup);
	public:
		PopupWindow();
		void Push(PopupData& PopupData);
		int size() const;
		void Update();
		static PopupWindow& GetGlobalPopup();
	};
}


