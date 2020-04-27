#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"
#include "Graphic/Camera.hpp"
#include "Core/EC/GameObject.hpp"
#include "Graphic/Texture.hpp"
#include <glm/ext.hpp>


#include <string.h>

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
		sr_texturePath = path;
		m_texture = new Graphic::Texture(newTexture);
	}

	////save data
	//sr_texturePath = path;
	////ENGINE_INFO("Texture Path: " + sr_texturePath);
	//texture = GLRenderer::GetInstance()->LoadTexture(path);

	//if (texture != -1)
	//	m_texLoaded = true;
	//else
	//	m_texLoaded = false;
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
	return (m_texture);
}

void  MeshRenderer::CreateMesh(float NumframeX, float NumFrameY)
{
	//save data
	sr_NumFrameX = NumframeX;
	sr_NumFrameY = NumFrameY;

	this->mesh = new SquareMeshVbo();
	this->mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);
}

void MeshRenderer::SetReplaceColor(glm::vec3 color) {
	this->ReplaceColor = color;
}

void MeshRenderer::SetReplaceColor(std::string hexcode) {
	//glm::vec3 smthign = hextovec3;
	//SetReplaceColor(smthing);
}

void MeshRenderer::RemoveReplaceColor() {
	ReplaceColor = glm::vec3(1);
}

void MeshRenderer::Render(glm::mat4 globalModelTransform)
{

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

	if (IsTextureLoaded())
	{
		modelMatrix *= glm::scale(glm::vec3(m_texture->m_size, 1.0f));
	}

	glm::mat4 currentMatrix = globalModelTransform * modelMatrix;

	/*
	if (!isUI){
		glm::mat4 modelMatrix = GetGameObject()->m_transform->GetModelMatrix();
		glm::mat4 projectionMatrix = Graphic::getCamera()->GetProjectionMatrix();
		glm::mat4 viewMatrix = Graphic::getCamera()->GetViewMatrix();

		currentMatrix = projectionMatrix * viewMatrix * modelMatrix;
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
		//if (m_texLoaded)
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


		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));

		//-------Animation--------

		if (GetGameObject()->GetComponent<Animator>() != nullptr)
		{
			glUniform1f(offsetXId, GetGameObject()->GetComponent<Animator>()->GetCurrentUVFrame().x);
			glUniform1f(offsetYId, GetGameObject()->GetComponent<Animator>()->GetCurrentUVFrame().y);

		}
		else {
			glUniform1f(offsetXId, 0.0f);
			glUniform1f(offsetYId, 0.0f);

		}

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
