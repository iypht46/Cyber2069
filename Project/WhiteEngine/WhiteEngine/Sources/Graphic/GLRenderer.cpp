
#include "GLRenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Factory.h"
#include "Core/Logger.hpp"
#include "Window.hpp"
#include <vector>
#include "Core/EC/Components/TextRenderer.hpp"
#include "Core/EC/Components/MeshRenderer.hpp"
#include "Core/EC/GameObject.hpp"

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

	return true;

}

void GLRenderer::Render()
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	// Update window with OpenGL rendering

	glUseProgram(gProgramId);

	this->PrintProgramLog(gProgramId);
	glm::mat4 camera = glm::mat4(1.0);

	//--------Render Object Here--------

	/*for (MeshRenderer *obj : Factory<MeshRenderer>::getCollection()) {
		
		if (obj->GetGameObject()->Active()) 
		{
			obj->Render(camera);
		}
	}*/

	for (MeshRenderer *obj : MeshSet) {

		if (obj->GetGameObject()->Active())
		{
			obj->Render(camera);
		}
	}

	for (TextRenderer *obj : Factory<TextRenderer>::getCollection()) {

		if (obj->GetGameObject()->Active())
		{
			obj->Render();
		}
	}

	//Unbind program
	glUseProgram(NULL);
}

void GLRenderer::CheckUnassignedLayer() 
{
	for (MeshRenderer *obj : Factory<MeshRenderer>::getCollection()) 
	{
		if (obj->layer == -1) 
		{
			ENGINE_WARN("Mesh Layer Unassigned (set to 0)");
			obj->SetLayer(0);
		}
	}
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(image);

	return texture;
}