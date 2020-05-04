#pragma once
#include "EditorObject/EditorObject.hpp"
#include "Core/Animation/Animation.hpp"
#include "Core/Animation/AnimationController.hpp"
#include "EditorObject/EditorObject.hpp"
#include "Core/Handle.hpp"
#include "Utility/Filesystem.hpp"

namespace Tools
{
	using AC_HANDLE = SHARED_HANDLE(AnimationController);
	using Path = Utility::fs::path;

	class EditorAC : public EditorObject
	{
	public:
		EditorAC();
		virtual void OnRender() override;
		virtual bool Load(Container::wString path) override;
		virtual bool Save(Container::wString path) override;
		AC_HANDLE GetAnimController();
		bool shouldUpdateMesh = false;
		int currentState = 0;
	private:
		AC_HANDLE m_animationController;
		glm::vec2 m_frameSize;

	};
}
