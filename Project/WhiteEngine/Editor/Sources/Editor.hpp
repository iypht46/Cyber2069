#pragma once

namespace Tools
{
	class Editor
	{
	public:
		virtual void Init(void) = 0;

		virtual void Update(void) = 0;

		virtual void Terminate(void) = 0;
	};

	void Init(void);

	void Loop(void);

	void Update(float dt);

	void Terminate(void);
}
