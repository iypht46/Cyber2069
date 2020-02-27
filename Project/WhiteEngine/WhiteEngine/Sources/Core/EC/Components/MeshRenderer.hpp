#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"

#include "Core/EC/Components/Animator.hpp"

class GameObject;
class Component;
namespace Tools { class MeshRendererEC; }

class MeshRenderer : public Component
{
	friend class Tools::MeshRendererEC;
private:
	unsigned int texture;
	MeshVbo* mesh;

	Animator* anim;

public:
	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
	~MeshRenderer();
	void CreateMesh(float NumframeX, float NumFrameY);
	void SetTexture(std::string path);
	void Render(glm::mat4 globalModelTransform);
	unsigned int GetTexture();
};
