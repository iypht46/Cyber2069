#pragma once
#include <string>
#include "imgui.h"

namespace Tools
{
	struct Funcs
	{
		static int MyResizeCallback(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
			{
				std::string* my_str = (std::string*)data->UserData;
				if (my_str->data() == data->Buf)
				{
					my_str->resize(data->BufSize);  // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
					data->Buf = (char*)my_str->data();
					return 0;
				}
				
			}
			return 0;
		}

		// Tip: Because ImGui:: is a namespace you would typicall add your own function into the namespace in your own source files.
		// For example, you may add a function called ImGui::InputText(const char* label, MyString* my_str).
		static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
		{
			IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
			return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
		}
	};
}