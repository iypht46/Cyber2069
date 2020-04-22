#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"
#include "Graphic/Camera.hpp"
#include "Core/EC/GameObject.hpp"

#include <string.h>

//bool operator < (const MeshRenderer &m1, const MeshRenderer &m2)
//{
//	return m1.layer < m2.layer;
//}

MeshRenderer::MeshRenderer() 
{
	ReplaceColor = glm::vec3(1, 1, 1);

	Factory<MeshRenderer>::Add(this);
}

MeshRenderer::~MeshRenderer()
{
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
	//save data
	sr_texturePath = path;

	texture = GLRenderer::GetInstance()->LoadTexture(path);
}

void MeshRenderer::SetTexture(unsigned int tex) 
{
	texture = tex;
}

void MeshRenderer::SetLayer(unsigned int layer) 
{
	this->layer = layer;
}

void MeshRenderer::SetUI(bool ui) 
{
	isUI = ui;
}

int MeshRenderer::GetLayer() 
{
	return this->layer;
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

	vector<glm::mat4> matrixStack;


	glm::mat4 currentMatrix;

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

	if (squareMesh != nullptr)
	{
		if (/*isReplaceColor*/ true) {

			glUniform1i(modeId, 3);
			glUniform3f(colorId,ReplaceColor.x, ReplaceColor.y, ReplaceColor.z);

		}
		else {
			glUniform1i(modeId, 1);
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

		glBindTexture(GL_TEXTURE_2D, this->texture);
		squareMesh->Render();
	}
}