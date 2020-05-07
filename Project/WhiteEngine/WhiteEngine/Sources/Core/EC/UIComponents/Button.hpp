#pragma once
#include "Core/EC/GameObject.hpp"
#include "Core/EC/Components/Component.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/UIComponents/TextRenderer.hpp"
#include "Core/Logger.hpp"
#include "Input/Input.hpp"

#include <memory>

#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>

enum BUTTON_TYPE {
	UICONTROL = 0,
	LOADOUTSELECT
};


struct OnHoverModifier {
	glm::vec3 ReColor = glm::vec3(1, 1, 1);
	string ReTexturePath = "";
	string ReText = "";
public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			ReColor,
			ReTexturePath,
			ReText
		);
	}
};

class Button : public BehaviourScript {
private:
	TextRenderer* text;
	MeshRenderer* mesh;
	glm::vec2 m_pos;

	int DefaultTexture = -1;
	string DefaultText = "";
	
	int OnHoverTex = -1;

public:
	OnHoverModifier hoverModifier;
	bool isOnHover = false;
	int buttonType = 0;
	int onClickType = 0;

	Button();

	void ModifyOnHover();

	void SetModifier(OnHoverModifier modifier) { this->hoverModifier = modifier; }

	virtual void OnAwake();
	virtual void OnUpdate(float dt);
	void OnClick();
	~Button();

public:
	template<class Archive>
	void serialize(Archive& archive) {
		archive(
			cereal::base_class<BehaviourScript>(this)
		);
	}
};

CEREAL_REGISTER_TYPE(Button);