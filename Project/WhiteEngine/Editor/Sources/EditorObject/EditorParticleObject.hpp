#pragma once

#include "EditorObject/EditorObject.hpp"
#include "Core/Particle/ParticleSystem.h"

namespace Tools
{
	class EditorParticleObject : public EditorObject
	{
	public:
		EditorParticleObject();
		virtual void OnRender() override;
		virtual bool Load(Container::wString path) override;
		virtual bool Save(Container::wString path) override;
	protected:

	private:

	};
}