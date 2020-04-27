#pragma once
#include "EditorObject/EditorObject.hpp"
#include "Core/Animation/Animation.hpp"
#include "Core/Animation/AnimationController.hpp"
#include "Core/Handle.hpp"
#include "Utility/Filesystem.hpp"

namespace Tools
{
	using AC_HANDLE = UNIQUE_HANDLE(AnimationController);
	using Path = Utility::fs::path;

	class EditorAC
	{
	public:
		EditorAC();
		virtual void OnRender();
		bool Load(Path path);
		bool Save(Path path);
	protected:
		
	private:
		AC_HANDLE m_animationController;
		glm::vec2 m_frameSize;

	};
}
