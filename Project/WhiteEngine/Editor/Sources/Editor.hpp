#pragma once

namespace Tools
{
	class Editor
	{
	public:
		void Init(void);

		void Loop(void);

		void Update(float dt);

		void Terminate(void);
	};

	void Init(void);

	void Loop(void);

	void Update(float dt);

	void Terminate(void);
}
