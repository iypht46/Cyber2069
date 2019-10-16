#pragma once

//System Headers
#include "Graphic/Window.hpp"
#include "Graphic/GLRenderer.h"

//Standard Header
#include <iostream>

namespace Graphic
{
	static GLRenderer* g_renderer;

	void Init(void);

	void Render(void);

	void Terminate(void);

}