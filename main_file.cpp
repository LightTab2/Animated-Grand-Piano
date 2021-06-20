/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "ObjMtlParser.h"

float speed_x=0;
float speed_y=0;
float aspectRatio=1;

ShaderProgram *sp;


LoadedObjModel* GrandPianoBase;
LoadedObjModel* GrandPianoStrings;
LoadedObjModel* GrandPianoLid;
LoadedObjModel* GrandPianoHammer[17];
const std::string GrandPianoHammerFiles[17] = { "GrandPianoHammer/Hammer_BlendSwap_000000.obj", "GrandPianoHammer/Hammer_BlendSwap_000001.obj","GrandPianoHammer/Hammer_BlendSwap_000002.obj","GrandPianoHammer/Hammer_BlendSwap_000003.obj","GrandPianoHammer/Hammer_BlendSwap_000004.obj",
"GrandPianoHammer/Hammer_BlendSwap_000005.obj","GrandPianoHammer/Hammer_BlendSwap_000006.obj","GrandPianoHammer/Hammer_BlendSwap_000007.obj","GrandPianoHammer/Hammer_BlendSwap_000008.obj","GrandPianoHammer/Hammer_BlendSwap_000009.obj",
"GrandPianoHammer/Hammer_BlendSwap_000010.obj","GrandPianoHammer/Hammer_BlendSwap_000011.obj","GrandPianoHammer/Hammer_BlendSwap_000012.obj","GrandPianoHammer/Hammer_BlendSwap_000013.obj","GrandPianoHammer/Hammer_BlendSwap_000014.obj","GrandPianoHammer/Hammer_BlendSwap_000015.obj","GrandPianoHammer/Hammer_BlendSwap_000016.obj" };
int GrandPianoHammerFrame[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

LoadedObjModel* Floor;
LoadedObjModel* ToyPianoBase;
LoadedObjModel* ToyPianoHammer;
glm::vec3 HammerPositions[8] =
{
	glm::vec3(-0.574033f, 0.564779f, 0.566373f),
	glm::vec3(-0.4389f, 0.564779f, 0.566373f),
	glm::vec3(-0.285781f, 0.564779f, 0.566373f),
	glm::vec3(-0.127831f, 0.564779f, 0.566373f),

	glm::vec3(0.051862f, 0.564779f, 0.566373f),
	glm::vec3(0.204517f, 0.564779f, 0.566373f),
	glm::vec3(0.366092f, 0.564779f, 0.566373f),
	glm::vec3(0.520791f, 0.564779f, 0.566373f)
};
float HammerAngles[8]=
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

LoadedObjModel* ToyPianoLid;
float lidAngle = 0.0f * PI / 180.0f;
double lidAnimationTime = 3;
double lidAnimationTimeEnd = 2;
bool lidOpen = false;

LoadedObjModel* ToyPianoButtons[8];
bool ToyPianoButtonsPress[8] = { false, false, false, false, false, false, false, false, };
double ButtonAnimationTime[8] = { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
double ButtonAnimationTimeEnd = 0.1f;
float ToyPianoButtonsDepth[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float ToyPianoBUttonDepthEnd = 0.066f;

glm::vec4 LightSource1 = glm::vec4(0, -0.3, 0,0);


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void PianoPress(int i = 0)
{
	ToyPianoButtonsPress[i] = true;
	if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
		ButtonAnimationTime[i] = ButtonAnimationTimeEnd - ButtonAnimationTime[i];
	else
		ButtonAnimationTime[i] = 0;
}
void PianoRelease(int i = 0)
{
	ToyPianoButtonsPress[i] = false;
	if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
		ButtonAnimationTime[i] = ButtonAnimationTimeEnd - ButtonAnimationTime[i];
	else
		ButtonAnimationTime[i] = 0;
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
		if (key == GLFW_KEY_O)
		{
			lidOpen = !lidOpen;
			if (lidAnimationTime <= lidAnimationTimeEnd)
				lidAnimationTime = lidAnimationTimeEnd - lidAnimationTime;
			else
				lidAnimationTime = 0;
		}
		if (key == GLFW_KEY_A)
		{
			PianoPress(0);
		}
		if (key == GLFW_KEY_S)
		{
			PianoPress(1);
		}
		if (key == GLFW_KEY_D)
		{
			PianoPress(2);
		}
		if (key == GLFW_KEY_F)
		{
			PianoPress(3);
		}
		if (key == GLFW_KEY_G)
		{
			PianoPress(4);
		}
		if (key == GLFW_KEY_H)
		{
			PianoPress(5);
		}
		if (key == GLFW_KEY_J)
		{
			PianoPress(6);
		}
		if (key == GLFW_KEY_K)
		{
			PianoPress(7);
		}
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
		if (key == GLFW_KEY_A)
		{
			PianoRelease(0);
		}
		if (key == GLFW_KEY_S)
		{
			PianoRelease(1);
		}
		if (key == GLFW_KEY_D)
		{
			PianoRelease(2);
		}
		if (key == GLFW_KEY_F)
		{
			PianoRelease(3);
		}
		if (key == GLFW_KEY_G)
		{
			PianoRelease(4);
		}
		if (key == GLFW_KEY_H)
		{
			PianoRelease(5);
		}
		if (key == GLFW_KEY_J)
		{
			PianoRelease(6);
		}
		if (key == GLFW_KEY_K)
		{
			PianoRelease(7);
		}
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);


	//LoadedObjModel** TempObjPtrArray[2] = { &GrandPianoBase, &GrandPianoLid };
	
	//loadMultipleObj("grandPiano.obj", TempObjPtrArray, 2);
	//loadObj("grandPianoStrings.obj", &GrandPianoStrings);
	//GrandPianoStrings->loadTex("StringsTexture.png");
	//GrandPianoStrings->loadTex_spec("StringsTexture.png");
	//GrandPianoLid->loadTex("TexturePiano.png");
	//GrandPianoLid->loadTex_spec("TexturePianoSpec.png");
	//loadAnimationObj(GrandPianoHammerFiles, GrandPianoHammer, 17);
	//GrandPianoHammer[0]->loadTex("TextureHammer.png");
	//GrandPianoHammer[0]->loadTex_spec("TextureHammer.png");



	loadObj("floor.obj", &Floor);
	Floor->loadTex("floor.png");
	Floor->loadTex_spec("floor.png");
	loadObj("base.obj", &ToyPianoBase);
	ToyPianoBase->loadTex("TextureBase.png");
	ToyPianoBase->loadTex_spec("TextureBase.png");
	loadObj("hammernlid.obj", &ToyPianoHammer);
	ToyPianoHammer->loadTex("TextureBase.png");
	ToyPianoHammer->loadTex_spec("TextureBase.png");
	loadObj("hammernlid.obj", &ToyPianoLid,1);
	ToyPianoLid->loadTex("TextureBase.png");
	ToyPianoLid->loadTex_spec("TextureBase.png");

	for (int i = 0; i < 8; i++)
	{
		loadObj("buttons.obj", &ToyPianoButtons[i],i);
		ToyPianoButtons[i]->loadTex("TextureBase.png");
	}

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}


void drawModel(LoadedObjModel* ptrModel)
{
	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, ptrModel->_vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, ptrModel->_normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texcoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, ptrModel->_texCoords);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ptrModel->_tex);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptrModel->_texspec);

	if (ptrModel->hasIndexes == 0)
		glDrawArrays(GL_TRIANGLES, 0, ptrModel->_verticesCount); //Narysuj obiekt
	else
		glDrawElements(GL_TRIANGLES, ptrModel->hasIndexes, GL_UNSIGNED_INT, ptrModel->_indexes);


	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void drawAnimatedModel(LoadedObjModel** ptrModel,int frame)
{
	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, ptrModel[frame]->_vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, ptrModel[frame]->_normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texcoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, ptrModel[frame]->_texCoords);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ptrModel[0]->_tex);

	glUniform1i(sp->u("textureMap1"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ptrModel[0]->_texspec);

	if (ptrModel[frame]->hasIndexes == 0)
		glDrawArrays(GL_TRIANGLES, 0, ptrModel[frame]->_verticesCount); //Narysuj obiekt
	else
		glDrawElements(GL_TRIANGLES, ptrModel[frame]->hasIndexes, GL_UNSIGNED_INT, ptrModel[frame]->_indexes);


	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 V = glm::lookAt(
		glm::vec3(0, 1.5, 3),
		glm::vec3(0, 0.5, 0),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	if(!lidOpen)
		V=glm::lookAt(
			glm::vec3(0, 1.5, 3),
			glm::vec3(0, 0.5, 0),
			glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku
	else
		V = glm::lookAt(
			glm::vec3(0, 1.5, -2.0),
			glm::vec3(0, 0.0, 1),
			glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 10000.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);

    sp->use();//Aktywacja programu cieniującego

	/*
	//PODSTAWA GRAND
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	drawModel(GrandPianoBase);

	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	//drawModel(GrandPianoStrings);
	
	//KLAPA GRAND
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source

	glm::mat4 Mlid = glm::translate(M, glm::vec3(-2.50711f, 2.81436f, 0.0f)); //Wylicz macierz modelu
	Mlid = glm::rotate(Mlid, lidAngle, glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mlid));
	//drawModel(GrandPianoLid);

	//Mloteczki
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	//drawAnimatedModel(GrandPianoHammer, GrandPianoHammerFrame[0]);
	*/

	//PODLOGA
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	drawModel(Floor);

	//PODSTAWA PIANINA
	glUniform4fv(sp->u("ls"),4, glm::value_ptr(LightSource1)); //light source
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

	drawModel(ToyPianoBase);
	
	//PRZYCISKI oraz MLOTKI
	for (int i = 0; i < 8; i++)
	{
		glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source

		glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
		glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
		glm::mat4 Mbutton = glm::translate(M, glm::vec3(0.0f, -ToyPianoButtonsDepth[i], 0.0f));
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mbutton));
		drawModel(ToyPianoButtons[i]);
		glm::mat4 Mhammer = glm::translate(M, HammerPositions[i]);
		Mhammer = glm::rotate(Mhammer, HammerAngles[i], glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mhammer));
		drawModel(ToyPianoHammer);
	}

	//KLAPA
	glUniform4fv(sp->u("ls"), 4, glm::value_ptr(LightSource1)); //light source

	glm::mat4 Mlid = glm::translate(M, glm::vec3(-0.644877f, 0.740097f, 0.0f)); //Wylicz macierz modelu
	Mlid = glm::rotate(Mlid, 15.0f*PI/180.0f, glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	Mlid = glm::rotate(Mlid, lidAngle, glm::vec3(0.0f, 0.0f, 1.0f)); //Wylicz macierz modelu
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(Mlid));
	drawModel(ToyPianoLid);
	




    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		//GrandPianoHammerFrame[0]++;
		//if (GrandPianoHammerFrame[0] > 16)GrandPianoHammerFrame[0] = 0;

		if (lidAnimationTime <= lidAnimationTimeEnd)
		{
			lidAnimationTime = lidAnimationTime + glfwGetTime();
			if (lidOpen)
			{
				if (lidAnimationTime >= lidAnimationTimeEnd)
					lidAngle = 105.0f * PI / 180.0f;
				else
					lidAngle = (0.0f + (105.0f * (lidAnimationTime / lidAnimationTimeEnd))) * PI / 180.0f;
			}
			else
			{
				if (lidAnimationTime >= lidAnimationTimeEnd)
					lidAngle = 0.0f * PI / 180.0f;
				else
					lidAngle = (105.0f - (105.0f * (lidAnimationTime / lidAnimationTimeEnd))) * PI / 180.0f;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (ButtonAnimationTime[i] <= ButtonAnimationTimeEnd)
			{
				ButtonAnimationTime[i] = ButtonAnimationTime[i] + glfwGetTime();
				if (ToyPianoButtonsPress[i])
				{
					if (ButtonAnimationTime[i] >= ButtonAnimationTimeEnd)
					{
						HammerAngles[i] = 13.5f * PI / 180.0f;
						ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd;
					}
					else
					{
						HammerAngles[i] = (0.0f + (13.5f * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd))) * PI / 180.0f;
						ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd);
					}
				}
				else
				{
					if (ButtonAnimationTime[i] >= ButtonAnimationTimeEnd)
					{
						HammerAngles[i] = 0.0f * PI / 180.0f;
						ToyPianoButtonsDepth[i] = 0;
					}
					else
					{
						HammerAngles[i] = (13.5f - (13.5f * (ButtonAnimationTime[i] / ButtonAnimationTimeEnd))) * PI / 180.0f;
						ToyPianoButtonsDepth[i] = ToyPianoBUttonDepthEnd * (1.0f-ButtonAnimationTime[i] / ButtonAnimationTimeEnd);
					}
				}
			}
		}

        glfwSetTime(0); //Zeruj timer
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
