#include "MeshRenderer.hpp"
#include "Graphic/SquareMeshVbo.h"
#include "Graphic/GLRenderer.h"

MeshRenderer::MeshRenderer() {

}

MeshRenderer::~MeshRenderer()
{
}



MeshRenderer::MeshRenderer(std::string texture_path,float NumframeX,float NumFrameY)
{
	//SetGameObject(m_gameObject);
	SetTexture(texture_path);

	mesh = new SquareMeshVbo();
	mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);

	Animation* Running = new Animation();

	Running->setStartPosition(0, 1);
	Running->setEndPosition(4, 1);

	AnimationController* RabbitController = new AnimationController();
	RabbitController->setSheetSize(glm::vec2(7, 5));
	RabbitController->AddState(Running);

	anim = new Animator();
	anim->AssignController(RabbitController);
	anim->setCurrentState(0);
}

void MeshRenderer::SetTexture(std::string path)
{
	texture = GLRenderer::GetInstance()->LoadTexture(path);
}

void  MeshRenderer::CreateMesh(float NumframeX, float NumFrameY)
{
	mesh = new SquareMeshVbo();
	mesh->LoadData(NumframeX, NumFrameY);
	GLRenderer::GetInstance()->SetMeshAttribId(mesh);
}

void MeshRenderer::Render(glm::mat4 globalModelTransform)
{
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *>(this->mesh);

	GLuint modelMatixId = GLRenderer::GetInstance()->GetModelMatrixAttrId();
	GLuint modeId = GLRenderer::GetInstance()->GetModeUniformId();

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

	
	//-------Transform--------
	glm::mat4 transform = glm::mat4(1.0);
		
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 size = glm::vec3(100.0, 100.0, 1.0);

	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::scale(transform, glm::vec3(size.x, size.y, 1));
	
	glm::mat4 currentMatrix = transform;
	//-----------------------------------

	//glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr)
	{
		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);

		//-------Animation--------

		glUniform1f(offsetXId, anim->GetCurrentUVFrame().x);
		glUniform1f(offsetYId, anim->GetCurrentUVFrame().y);

		/*glUniform1f(offsetXId, 0);
		glUniform1f(offsetYId, 0);*/

		glBindTexture(GL_TEXTURE_2D, texture);
		squareMesh->Render();
	}
}

void MeshRenderer::OnAwake()
{
}

void MeshRenderer::OnEnable()
{
}

void MeshRenderer::OnStart()
{
}

void MeshRenderer::OnDisable()
{
}

void MeshRenderer::OnUpdate(float dt)
{
}

void MeshRenderer::OnFixedUpdate(float dt)
{
}

void MeshRenderer::OnDestroy()
{
}