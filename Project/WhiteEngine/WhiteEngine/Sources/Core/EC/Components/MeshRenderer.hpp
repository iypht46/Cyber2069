#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>

#include "Core/EC/Components/Animator.hpp"

#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/base_class.hpp>

class GameObject;
class Component;
namespace Graphic { struct Texture; }

enum RENDER_MODE : int { NORM = 1, COLOR, REPLACE_COLOR};

#define SpritePath(path) "Sources/Assets/Sprites/" path ".png"

class MeshRenderer : public Component
{
private:
	bool m_isUI = false;

	std::string sr_texturePath = "none";
	float sr_NumFrameX = 1.0f;
	float sr_NumFrameY = 1.0f;

	bool isReplaceColor = false;
	MeshVbo* mesh = nullptr;
	Animator* anim = nullptr;
	GLuint texture = -1;
	bool m_texLoaded = false;
	glm::vec3 ReplaceColor;

	Graphic::Texture* m_texture = nullptr;
public:
	int layer = 0; //higher layer is infront
	bool inSet = false;

	MeshRenderer();
	MeshRenderer(std::string texture_path, float NumframeX, float NumFrameY);
	~MeshRenderer();

	virtual void Init();

	void CreateMesh(float NumframeX, float NumFrameY);
	void SetTexture(std::string path);
	void SetLayer(unsigned int layer); //higher layer is in front, player is on 0
	void SetUI(bool ui);
	void SetReplaceColor(glm::vec3 color);
	void SetReplaceColor(std::string hexcode);
	void SetReplaceColorBool(bool i);
	glm::vec3 GetReplaceColor();
	bool IsReplaceColor();
	void RemoveReplaceColor();

	int GetLayer();
	bool IsUI();
	bool IsTextureLoaded();

	void Render(glm::mat4 globalModelTransform);
	unsigned int GetTexture();
//serialization
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<Component>(this),
			m_isUI,
			m_texLoaded,
			sr_texturePath,
			sr_NumFrameX,
			sr_NumFrameY,
			layer,
			ReplaceColor,
			inSet
		);
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer);
