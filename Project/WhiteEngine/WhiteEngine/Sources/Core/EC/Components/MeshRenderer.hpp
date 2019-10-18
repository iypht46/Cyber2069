#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"

#include "Core/Animator/Animator.hpp"

class MeshRenderer : public Component
{
private:
	unsigned int texture;
	MeshVbo* mesh;

	Animator* anim;

public:
	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
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
