#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Component.hpp"
#include "Graphic/MeshVbo.h"
#include <iostream>
#include <map>

#include "ft2build.h"
#include FT_FREETYPE_H

class GameObject;
class Component;

#ifndef GLCHARACTER
#define GLCHARACTER
struct GLCharacter {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};
#endif

class TextRenderer : public Component 
{
protected:
	GLuint VAO;
	GLuint VBO;
	glm::vec3 color;
	string text;

	std::map<GLchar, GLCharacter> Characters;
public:
	void Render();
	
	void SetText(string text);
	void SetColor(glm::vec3 color);
	void LoadFont(string path,float DefautFontSize);

	TextRenderer();
};
