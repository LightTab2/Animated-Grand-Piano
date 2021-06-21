#include "render.h"
#include "lilPiano.h"
#include "grandPiano.h"

float aspectRatio = 1.f;

void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	if (!height) return;

	glViewport(0, 0, width, height);
}

void RenderingSystem::LoadModels()
{
	LoadedModel::loadObj("Floor.obj", Floor);
	Floor->loadTex("Floor.png");

	LoadedModel::loadObj("base.obj", ToyPianoBase);
	ToyPianoBase->loadTex("TextureBase.png");

	LoadedModel::loadObj("hammernlid.obj", ToyPianoHammer);
	ToyPianoHammer->loadTex("TextureBase.png");

	LoadedModel::loadObj("hammernlid.obj", ToyPianoLid, 1);
	ToyPianoLid->loadTex("TextureBase.png"); 

	LoadedModel::loadObj("grandPiano.obj", GrandPianoBase);
	LoadedModel::loadObj("grandPianoLid.obj", GrandPianoLid);
	LoadedModel::loadObj("grandPianoStrings.obj", GrandPianoStrings);

	GrandPianoBase->loadTex("grand.png");
	GrandPianoLid->loadTex("grandLid.png");
	GrandPianoStrings->loadTex("grand2.png");

	LoadedModel::loadObj("leftKey.obj", leftKey);
	LoadedModel::loadObj("rightKey.obj", rightKey);
	LoadedModel::loadObj("middleKey.obj", middleKey);
	leftKey->loadTex("leftKey.png");
	rightKey->loadTex("rightKey.png");
	middleKey->loadTex("middleKey.png");
	LoadedModel::loadObj("blackKey.obj", blackKey);
	blackKey->loadTex("blackKey.png");

	for (unsigned i = 0; i != 22; ++i)
	{
		std::string file = "GrandPianoHammer/Hammer_BlendSwap_0000";
		LoadedModel::loadObj(file + std::to_string(i + 16) + ".obj", GrandPianoHammer[i]);
		if (i == 0)
			GrandPianoHammer[0]->loadTex("grandHammer.png");
		else
			GrandPianoHammer[i]->setTexture(GrandPianoHammer[0]->getTexture());
	}

	for (int i = 0; i != 8; ++i)
	{
		LoadedModel::loadObj("buttons.obj", ToyPianoButtons[i], i);
		ToyPianoButtons[i]->loadTex("TextureBase.png");
	}
}

void RenderingSystem::drawAnimatedModel(LoadedModel** ptrModel, int frame)
{
	glEnableVertexAttribArray(sp->a("vertex")); 
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, ptrModel[frame]->vertices);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, ptrModel[frame]->normals);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, ptrModel[frame]->texCoords);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ptrModel[0]->tex);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptrModel[0]->texSpec);

	if (ptrModel[frame]->hasIndexes == 0)
		glDrawArrays(GL_TRIANGLES, 0, ptrModel[frame]->verticesCount);
	else
		glDrawElements(GL_TRIANGLES, ptrModel[frame]->hasIndexes, GL_UNSIGNED_INT, ptrModel[frame]->indexes);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void RenderingSystem::drawModel(LoadedModel* ptrModel)
{
	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, ptrModel->vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, ptrModel->normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texcoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, ptrModel->texCoords);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ptrModel->tex);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptrModel->texSpec);

	if (ptrModel->hasIndexes == 0)
		glDrawArrays(GL_TRIANGLES, 0, ptrModel->verticesCount); //Narysuj obiekt
	else
		glDrawElements(GL_TRIANGLES, ptrModel->hasIndexes, GL_UNSIGNED_INT, ptrModel->indexes);


	glDisableVertexAttribArray(sp->a("vertex"));	//Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));		//Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));	//Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

RenderingSystem::RenderingSystem()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		std::cerr << "Nie mozna zainicjowac GLFW." << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwSetTime(0);

	window = glfwCreateWindow(1360, 1024, "OpenGL", NULL, NULL);

	if (!window)
	{
		std::cerr << "Nie mozna utworzyc okna." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Nie mozna zainicjowac GLEW.\n");
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	glfwSetWindowSizeCallback(window, windowResizeCallback);

	glfwSetKeyCallback(window, keyCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetCursorPos(window, 0, 0);

	sp = new ShaderProgram("vertex.glsl", NULL, "fragment.glsl");

	LoadModels();
}

RenderingSystem::~RenderingSystem()
{
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glfwDestroyWindow(window);
	glfwTerminate();
	delete sp;
}

bool RenderingSystem::shouldClose() 
{
	return glfwWindowShouldClose(window);
}

void RenderingSystem::getCursorPos(double& angle_y, double& angle_x)
{
	glfwGetCursorPos(window, &angle_y, &angle_x);
}

void RenderingSystem::setCursorPos(double angle_y, double angle_x)
{
	glfwSetCursorPos(window, angle_y, angle_x);
}