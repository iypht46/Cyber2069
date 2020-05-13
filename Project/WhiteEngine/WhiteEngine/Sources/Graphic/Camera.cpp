#include "Camera.hpp"

namespace Graphic
{
	CameraObject* getCamera()
	{
		if (g_camera == nullptr)
		{
			g_camera = new CameraObject();
		}
		return g_camera;
	}
}