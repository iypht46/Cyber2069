#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"

class MeshRenderer : public Component
{
private:
	unsigned int texture;
	MeshVbo* mesh;

public:
	MeshRenderer(std::string texture_path);
	~MeshRenderer();
	void SetTexture(std::string path);
	void Render(glm::mat4 globalModelTransform);

	virtual void OnAwake();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnStart();
	virtual void OnUpdate(float dt);
	virtual void OnFixedUpdate(float dt);
	virtual void OnDestroy();
};
