#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"
#include "Graphic/Camera.hpp"
#include "Core/EC/GameObject.hpp"
#include "Graphic/Texture.hpp"
#include <glm/ext.hpp>

#include <string.h>
#include <iostream>
#include <glm/gtx/matrix_interpolation.hpp>
//bool operator < (const MeshRenderer &m1, const MeshRenderer &m2)
//{
//	return m1.layer < m2.layer;
//}

MeshRenderer::MeshRenderer()
{
	ReplaceColor = glm::vec3(1, 1, 1);

	Factory<Component, MeshRenderer>::Add(this);
}

MeshRenderer::~MeshRenderer()
{
	Factory<Component, MeshRenderer>::Remove(this);
}


MeshRenderer::MeshRenderer(std::string texture_path,float NumframeX,float NumFrameY)
{
	//save data
	sr_texturePath = texture_path;
	sr_NumFrameX = NumframeX;
	sr_NumFrameY = NumFrameY;

	SetTexture(texture_path);

	mesh = new SquareMeshVbo();
	mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);

}

void MeshRenderer::Init() {
	//ENGINE_INFO("{}", sr_texturePath);
	SetTexture(sr_texturePath);

	mesh = new SquareMeshVbo();
	//ENGINE_INFO("numframe {},{}", sr_NumFrameX, sr_NumFrameY);
	//ENGINE_INFO("scale {}, pos {}", GetGameObject()->m_transform->GetScale().x, GetGameObject()->m_transform->GetLocalPosition().x);
	mesh->LoadData(sr_NumFrameX, sr_NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);

	anim = GetGameObject()->GetComponent<Animator>();
}

void MeshRenderer::SetTexture(std::string path)
{
	if (path == "none")
		return;

	auto newTexture = GLRenderer::GetInstance()->LoadTextureNew(path);

	if (newTexture.m_textureID != -1)
	{
		SetTexture(newTexture);
	}

	sr_texturePath = path;

	////save data
	//sr_texturePath = path;
	////ENGINE_INFO("Texture Path: " + sr_texturePath);
	//texture = GLRenderer::GetInstance()->LoadTexture(path);

	//if (texture != -1)
	//	m_texLoaded = true;
	//else
	//	m_texLoaded = false;
}

void MeshRenderer::SetTexture(Graphic::Texture tex)
{
	if (tex.m_textureID != -1)
	{
		delete m_texture;
		m_texture = new Graphic::Texture(tex);
		GetGameObject()->m_transform->SetMeshScale(glm::vec3(tex.m_size / glm::vec2(sr_NumFrameX, sr_NumFrameY), 1.0f));
	}
}

void MeshRenderer::SetLayer(unsigned int layer)
{
	this->layer = layer;
}

void MeshRenderer::SetUI(bool ui)
{
	m_isUI = ui;
}

int MeshRenderer::GetLayer()
{
	return this->layer;
}

bool MeshRenderer::IsUI()
{
	return m_isUI;
}

bool MeshRenderer::IsTextureLoaded()
{
	return (sr_texturePath != "none");
}

void  MeshRenderer::CreateMesh(float NumframeX, float NumFrameY)
{
	//save data
	sr_NumFrameX = NumframeX;
	sr_NumFrameY = NumFrameY;

	if (!mesh)
	{
		this->mesh = new SquareMeshVbo();
		this->mesh->LoadData(NumframeX, NumFrameY);
	}
	else
	{
		SquareMeshVbo* sm = dynamic_cast<SquareMeshVbo*>(mesh);
		if (sm)
			sm->ReloadTextureData(NumframeX, NumFrameY);
	}
	
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);
}

void MeshRenderer::SetReplaceColor(glm::vec3 color) {
	isReplaceColor = true;
	this->ReplaceColor = color;
}

void MeshRenderer::SetReplaceColor(std::string hexcode) {
	//glm::vec3 smthign = hextovec3;
	//SetReplaceColor(smthing);
}

void MeshRenderer::SetReplaceColorBool(bool i)
{
	isReplaceColor = i;
}

glm::vec3 MeshRenderer::GetReplaceColor()
{
	return ReplaceColor;
}

bool MeshRenderer::IsReplaceColor()
{
	return isReplaceColor;
}

void MeshRenderer::RemoveReplaceColor() {
	ReplaceColor = glm::vec3(1);
}

void MeshRenderer::Render(glm::mat4 globalModelTransform)
{
	if (!m_gameObject->Active())
		return;

	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *>(this->mesh);

	GLuint modelMatixId = GLRenderer::GetInstance()->GetModelMatrixAttrId();
	GLuint modeId = GLRenderer::GetInstance()->GetModeUniformId();
	GLuint vmodeId = GLRenderer::GetInstance()->GetvModeUniformId();
	GLuint colorId = GLRenderer::GetInstance()->GetColorUniformId();

	GLuint offsetXId = GLRenderer::GetInstance()->GetOffsetXUniformId();
	GLuint offsetYId = GLRenderer::GetInstance()->GetOffsetYUniformId();

	if (modelMatixId == -1)
	{
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (modeId == -1)
	{
		cout << "Error: Can't set mode in ImageObject " << endl;
		return;
	}

	//vector<glm::mat4> matrixStack;
	glm::mat4 modelMatrix = GetGameObject()->m_transform->GetModelMatrix();

	/*
	if (!isUI){
		glm::mat4 modelMatrix = GetGameObject()->m_transform->GetModelMatrix();
		glm::mat4 projectionMatrix = Graphic::getCamera()->GetProjectionMatrix();
		glm::mat4 viewMatrix = Graphic::getCamera()->GetViewMatrix();

		currentMatrix = projectionMatrix * glm::interpolate(viewMatrix, glm::mat4(1.0), GetGameObject()->m_transform->GetParallaxValue()) * modelMatrix;
	}
	else {
		glm::mat4 modelMatrix = GetGameObject()->m_transform->GetModelMatrix();
		glm::mat4 projectionMatrix = GLRenderer::GetInstance()->GetprojectionMatrix();

		currentMatrix = projectionMatrix * modelMatrix;
	}
	*/
	if (squareMesh != nullptr)
	{
		if (this->IsTextureLoaded())
		{
			glBindTexture(GL_TEXTURE_2D, m_texture->m_textureID);
			//glBindTexture(GL_TEXTURE_2D, texture);
			if (isReplaceColor)
			{
				glUniform1i(modeId, RENDER_MODE::REPLACE_COLOR);
				glUniform3f(colorId, ReplaceColor.x, ReplaceColor.y, ReplaceColor.z);
			}
			else
			{
				glUniform1i(modeId, RENDER_MODE::NORM);
			}
		}
		else
		{
			glUniform1i(modeId, RENDER_MODE::COLOR);
		}

		glm::vec2 textureScale;

		if (IsTextureLoaded())
			textureScale = m_texture->m_size;

		//-------Animation--------
		auto animation = GetGameObject()->GetComponent<Animator>();

		if (animation && animation->GetController().get())
		{
			auto uv = animation->GetCurrentUVFrame();
			textureScale = textureScale / animation->GetController()->getSheetSize(); //Divide by frame size to get the unit texture size
			glUniform1f(offsetXId, uv.x);
			glUniform1f(offsetYId, uv.y);
			//glUniform1f(offsetXId, 0.0f);
			//glUniform1f(offsetYId, 0.0f);
		}
		else 
		{
			textureScale = textureScale / glm::vec2(sr_NumFrameX, sr_NumFrameY);
			glUniform1f(offsetXId, meshUV.x / sr_NumFrameX);
			glUniform1f(offsetYId, meshUV.y / sr_NumFrameY);
		}

		//ENGINE_INFO("Texture Scale : {}, {}", textureScale.x, textureScale.y);
		if (IsTextureLoaded() && !m_isUI)
		{
			auto meshMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(textureScale, 1.0f));
			
			modelMatrix *= glm::interpolate(glm::mat4(1.0f), glm::translate(glm::mat4(1.0f), glm::vec3(Graphic::getCamera()->GetCampos().x, Graphic::getCamera()->GetCampos().y, 0)), GetGameObject()->m_transform->GetParallaxValue()) * meshMatrix;
			
		}
			
		//std::cout << glm::to_string(globalModelTransform) << std::endl << std::endl;
		glm::mat4 currentMatrix = globalModelTransform * modelMatrix;

		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));

		/*glUniform1f(offsetXId, 0);
		glUniform1f(offsetYId, 0);*/
		//std::cout << "Render\n";

		/*if (IsTextureLoaded())
			glBindTexture(GL_TEXTURE_2D, m_texture->m_textureID);*/

		squareMesh->Render();
	}
}

unsigned int MeshRenderer::GetTexture()
{
	if (m_texture)
		return m_texture->m_textureID;
	else
		return -1;
}

Graphic::Texture MeshRenderer::GetTextureObj()
{
	return *m_texture;
}

std::string MeshRenderer::GetTexturePath()
{
	return sr_texturePath;
}
