
#include "GLRenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Factory.h"
#include "Core/Logger.hpp"
#include "Window.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "Core/EC/UIComponents/TextRenderer.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/GameObject.hpp"
#include "Graphic/Camera.hpp"
#include <glm/gtx/string_cast.hpp>
using namespace std;

GLRenderer *GLRenderer::instance = nullptr;

GLRenderer* GLRenderer::GetInstance() {
	if (instance == nullptr)
	{
		instance = new GLRenderer(Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
	}
	return instance;
}

GLRenderer::GLRenderer(int w, int h)
{
	this->winWidth = w;
	this->winHeight = h;
	glViewport(0, 0, winWidth, winHeight);
}

bool GLRenderer::InitGL(string vertexShaderFile, string fragmentShaderFile)
{
	//Initialize OpenGL
	if (!Initialize(vertexShaderFile, fragmentShaderFile))
	{
		cout << "Unable to initialize OpenGL! " << endl;
		return false;
	}

	return true;
}

bool GLRenderer::Initialize(string vertexShaderFile, string fragmentShaderFile)
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramId = glCreateProgram();
	vertexShader = new Shader(vertexShaderFile, GL_VERTEX_SHADER);
	if (!vertexShader->LoadSource()) {
		return false;
	}
	fragmentShader = new Shader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	if (!fragmentShader->LoadSource()) {
		return false;
	}
	glAttachShader(gProgramId, vertexShader->getShaderId());
	glAttachShader(gProgramId, fragmentShader->getShaderId());


	glLinkProgram(gProgramId);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(gProgramId2, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		cout << "Error linking program " << gProgramId2 << endl;
		PrintProgramLog(gProgramId2);
		return false;
	}

	programSuccess = GL_TRUE;
	glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		cout << "Error linking program " << gProgramId << endl;
		PrintProgramLog(gProgramId);
		return false;
	}

	//Get vertex attribute location
	gPos2DLocation = glGetAttribLocation(gProgramId, "pos2D");
	if (gPos2DLocation == -1)
	{
		cout << "pos2D is not a valid glsl program variable" << endl;
		return false;
	}
	gTex2DLocation = glGetAttribLocation(gProgramId, "inTexCoord");
	if (gTex2DLocation == -1)
	{
		cout << "inTexCoord is not a valid glsl program variable" << endl;
		return false;
	}

	//Setting color uniform id
	colorUniformId = glGetUniformLocation(gProgramId, "color");
	if (colorUniformId == -1)
	{
		cout << "color is not a valid glsl uniform variable" << endl;
		return false;
	}

	////Set up uniform id attribute
	///*pMatrixId = glGetUniformLocation(gProgramId, "pMatrix");
	//if (pMatrixId == -1)
	//{
	//	cout << "pMatrix is not a valid glsl uniform variable" << endl;
	//	return false;
	//}*/
	mMatrixId = glGetUniformLocation(gProgramId, "mMatrix");
	if (mMatrixId == -1)
	{
		cout << "mMatrix is not a valid glsl uniform variable" << endl;
		return false;
	}
	modeUniformId = glGetUniformLocation(gProgramId, "mode");
	if (modeUniformId == -1)
	{
		cout << "mode is not a valid glsl uniform variable" << endl;
		return false;
	}
	/*vmodeUniformId = glGetUniformLocation(gProgramId, "vmode");
	if (vmodeUniformId == -1)
	{
		cout << "mode is not a valid glsl uniform variable" << endl;
		return false;
	}*/
	offSetXId = glGetUniformLocation(gProgramId, "offsetX");
	if (modeUniformId == -1)
	{
		cout << "offsetX is not a valid glsl uniform variable" << endl;
		return false;
	}
	offSetYId = glGetUniformLocation(gProgramId, "offsetY");
	if (modeUniformId == -1)
	{
		cout << "offsetY is not a valid glsl uniform variable" << endl;
		return false;
	}

	glEnableVertexAttribArray(gPos2DLocation);
	glEnableVertexAttribArray(gTex2DLocation);

	//Initialize clear color
	glClearColor(1.0f, 1.0f, 1.0f, 1.f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLfloat vertexData[] =
	{
	  -0.5f, -0.5f,
	  0.5f, -0.5f,
	  0.5f,  0.5f,
	  -0.5f,  0.5f
	};

	glGenVertexArrays(1, &(this->colVAO));
	glGenBuffers(1, &(this->colVBO));

	//Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->colVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	vector<float> circleVertexData;
	for (int i = 0; i < 360; i++) {
		float rad;
		rad = glm::radians((float)i);

		float x = 0.5f * cos(rad);
		float y = 0.5f * sin(rad);

		circleVertexData.push_back(x);
		circleVertexData.push_back(y);
	}

	glGenVertexArrays(1, &(this->cirVAO));
	glGenBuffers(1, &(this->cirVBO));

	//Create VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->cirVBO);
	glBufferData(GL_ARRAY_BUFFER, circleVertexData.size() * sizeof(GLfloat), &circleVertexData.front(), GL_STATIC_DRAW);

	return true;

}

void GLRenderer::Render(Graphic::CameraObject* cam)
{
	AssignLayer();

	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);

	// Update window with OpenGL rendering
	glUseProgram(gProgramId); //Use gameobject shader program

	this->PrintProgramLog(gProgramId);

	//--------Render Object Here--------
	for (MeshRenderer* obj : MeshSet) {

		if (obj->GetGameObject()->Active() && obj->enabled)
		{
			//If object is flag as UI then reder with projection matrix
			if (obj->IsUI())
				obj->Render(GetprojectionMatrix());
			//Else render with projection * view matrix
			else
				obj->Render(cam->GetProjViewMatrix());
		}
	}

	for (auto obj : Factory<Component, TextRenderer>::getCollection()) {

		if (obj.second->GetGameObject()->Active())
		{
			obj.second->Render();
		}
	}

	if (drawDebug) {
		for (auto obj : Factory<Component, BoxCollider>::getCollection())
		{
			if (obj.second->GetGameObject()->Active())
			{
				RenderDebugCollider(obj.second, cam->GetProjViewMatrix());
			}
		}

		while (!Lineq.empty())
		{
			RenderLine(Lineq.front());
			Lineq.pop();
		}

		while (!Circleq.empty())
		{
			RenderCircle(Circleq.front());
			Circleq.pop();
		}
	}

	while (!Lineq.empty())
	{
		Lineq.pop();
	}

	while (!Circleq.empty())
	{
		Circleq.pop();
	}

}

void GLRenderer::Render(glm::mat4 globalModelTransform)
{
	AssignLayer();

	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);

	// Update window with OpenGL rendering
	glUseProgram(gProgramId); //Use gameobject shader program

	this->PrintProgramLog(gProgramId);

	//Set up matrix uniform
	glm::mat4 camera = glm::mat4(1.0f);

	//--------Render Object Here--------
	for (MeshRenderer *obj : MeshSet) {

		if (obj->GetGameObject()->Active())
		{
			obj->Render(globalModelTransform);
		}
	}

	for (auto obj : Factory<Component, TextRenderer>::getCollection()) {

		if (obj.second->GetGameObject()->Active())
		{
			obj.second->Render();
		}
	}

	if (drawDebug) {
		for (auto obj : Factory<Component, BoxCollider>::getCollection())
		{
			if (obj.second->GetGameObject()->Active())
			{
				RenderDebugCollider(obj.second, globalModelTransform);
			}
		}

		while (!Lineq.empty())
		{
			RenderLine(Lineq.front());
			Lineq.pop();
		}

		while (!Circleq.empty())
		{
			RenderCircle(Circleq.front());
			Circleq.pop();
		}
	}

	while (!Lineq.empty())
	{
		Lineq.pop();
	}

	while (!Circleq.empty())
	{
		Circleq.pop();
	}

}

void GLRenderer::AssignLayer()
{
	for (auto obj : Factory<Component, MeshRenderer>::getCollection())
	{
		if (obj.second->inSet == false) 
		{
			obj.second->inSet = true;

			//if (obj->layer == -1)
			//{
				//ENGINE_WARN("GameObjectMeshLayer unassigned (set Layer to 0)");
				//obj->SetLayer(0);
			//}

			GLRenderer::GetInstance()->AddMeshToSet(obj.second);
		}
	}
}

void GLRenderer::ResetAssignLayer()
{
	

	for (auto mesh : MeshSet)
	{
		mesh->inSet = false;
	}
	MeshSet.clear();
	AssignLayer();
}

void GLRenderer::SetAsgnLayer(bool asgn)
{
	this->AsgnLayer = asgn;
}

void GLRenderer::SetMeshAttribId(MeshVbo * mesh)
{
	mesh->SetAttribId(gPos2DLocation, gTex2DLocation);
}

void GLRenderer::PrintProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			cout << infoLog << endl;

		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

GLRenderer::~GLRenderer()
{
	if (gPos2DLocation != -1) {
		glDisableVertexAttribArray(gPos2DLocation);
	}

}

void GLRenderer::SetOrthoProjection(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top);
}

void GLRenderer::SetViewPort(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void GLRenderer::SetClearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0);
}

void GLRenderer::SetClearColor(float r, float g, float b, float w)
{
	glClearColor(r, g, b, w);
}

void GLRenderer::SetDefaultViewport()
{
	this->SetViewPort(0, 0, Graphic::Window::GetWidth(), Graphic::Window::GetHeight());
}

void GLRenderer::AddMeshToSet(MeshRenderer* mesh)
{
	//MeshSet.erase(mesh);

	MeshSet.insert(mesh);

}

glm::mat4 GLRenderer::GetprojectionMatrix()
{
	return projectionMatrix;
}

GLuint GLRenderer::GetModelMatrixAttrId()
{
	return this->mMatrixId;
}

GLuint GLRenderer::GetProjectionMatrixAttrId()
{
	return this->pMatrixId;
}

GLuint GLRenderer::GetColorUniformId()
{
	return this->colorUniformId;
}

GLuint GLRenderer::GetModeUniformId()
{
	return this->modeUniformId;
}

GLuint GLRenderer::GetvModeUniformId()
{
	return this->vmodeUniformId;
}

GLuint GLRenderer::GetOffsetXUniformId()
{
	return this->offSetXId;
}

GLuint GLRenderer::GetOffsetYUniformId()
{
	return this->offSetYId;
}

GLuint GLRenderer::GetProgramId() {
	return this->gProgramId;
}

int GLRenderer::GetgTex2DLocation() {
	return this->gTex2DLocation;
}

int GLRenderer::GetgPos2DLocation() {
	return this->gPos2DLocation;
}

GLuint GLRenderer::LoadTexture(string path)
{
	glActiveTexture(GL_TEXTURE0);
	SDL_Surface *image = IMG_Load(path.c_str());


	if (image == NULL)
	{
		cerr << "IMG_Load: " << SDL_GetError() << endl;
		return -1;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int Mode = GL_RGB;
	if (image->format->BytesPerPixel == 4)
	{
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(image);

	return texture;
}

Graphic::Texture GLRenderer::LoadTextureNew(std::string path)
{
	glActiveTexture(GL_TEXTURE0);
	SDL_Surface* image = IMG_Load(path.c_str());


	if (image == NULL)
	{
		cerr << "IMG_Load: " << SDL_GetError() << endl;
		return Graphic::Texture();
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	Graphic::Texture textureObject(texture, image->w, image->h);

	int Mode = GL_RGB;
	if (image->format->BytesPerPixel == 4)
	{
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(image);

	return textureObject;
}

void GLRenderer::RenderDebugCollider(BoxCollider* col, glm::mat4 projViewMat)
{
	GameObject* obj = col->GetGameObject();

	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), glm::vec3(col->GetHw() * 2, col->GetHh() * 2, 1.0f));
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), obj->m_transform->GetLocalPosition());
	glm::mat4 transformMat = tMat * sMat;

	//glm::mat4 projectionMatrix = Graphic::getCamera()->GetProjectionMatrix();
	//glm::mat4 viewMatrix = Graphic::getCamera()->GetViewMatrix();

	glm::mat4 currentMatrix = projViewMat * transformMat;//projectionMatrix * viewMatrix * transformMat;

	glUniform1i(modeUniformId, 0);
	glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
	glUniform3f(colorUniformId, 0.0f, 1.0f, 0.0f);

	glBindVertexArray(this->colVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->colVBO);
	glVertexAttribPointer(this->gPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINE_LOOP, 0, 4);

}

void GLRenderer::RenderLine(LineVertex* vertex)
{
	float x1 = vertex->x1;
	float y1 = vertex->y1;
	float x2 = vertex->x2;
	float y2 = vertex->y2;
	float length = glm::sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));

	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), glm::vec3(length, 1.0f, 1.0f));
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), glm::atan((y2 - y1) / (x2 - x1)), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), glm::vec3((x2 + x1) / 2, (y2 + y1) / 2, 1.0f));
	glm::mat4 transformMat = tMat * rMat * sMat;

	glm::mat4 projectionMatrix = Graphic::getCamera()->GetProjectionMatrix();
	glm::mat4 viewMatrix = Graphic::getCamera()->GetViewMatrix();

	glm::mat4 currentMatrix = projectionMatrix * viewMatrix * transformMat;

	glUniform1i(modeUniformId, 0);
	glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
	glUniform3f(colorUniformId, vertex->r, vertex->g, vertex->b);

	glBindVertexArray(this->colVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->colVBO);
	glVertexAttribPointer(this->gPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINES, 0, 2);
}

void GLRenderer::RenderCircle(CircleVertex* vertex) {
	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), glm::vec3(vertex->radius * 2, vertex->radius * 2, 1.0f));
	glm::mat4 tMat = glm::translate(glm::mat4(1.0f), glm::vec3(vertex->x, vertex->y, 1.0f));
	glm::mat4 transformMat = tMat * sMat;

	glm::mat4 projectionMatrix = Graphic::getCamera()->GetProjectionMatrix();
	glm::mat4 viewMatrix = Graphic::getCamera()->GetViewMatrix();

	glm::mat4 currentMatrix = projectionMatrix * viewMatrix * transformMat;

	glUniform1i(modeUniformId, 0);
	glUniformMatrix4fv(mMatrixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
	glUniform3f(colorUniformId, vertex->r, vertex->g, vertex->b);

	glBindVertexArray(this->cirVAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->cirVBO);
	glVertexAttribPointer(this->gPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINE_LOOP, 0, 360);
}

void GLRenderer::DrawDebug_Line(float x1, float y1, float x2, float y2,float r, float g, float b)
{
	Lineq.push(new LineVertex(x1, y1, x2, y2, r, g, b));
}

void GLRenderer::DrawDebug_Circle(float x, float y, float radius, float r, float g, float b) {
	Circleq.push(new CircleVertex(x, y, radius, r, g, b));
}
