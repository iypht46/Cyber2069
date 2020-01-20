#pragma once

namespace Tools
{
	class Editor
	{
	public:
		virtual void Init(void);

		virtual void Loop(void) = 0;

		virtual void Update(float dt) = 0;

		virtual void Terminate(void) = 0;
	};
}
