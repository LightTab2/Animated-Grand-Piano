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

#include "shaderprogram.h"

//Procedura wczytuje plik do tablicy znaków (bajtów)
char* ShaderProgram::readFile(const char* fileName)
{
#pragma warning(suppress : 6386) //Wyłączenie błędu w Visual Studio wynikającego z błędnej analizy statycznej kodu
	int filesize;
	FILE* file;
	char* result;

#pragma warning(suppress : 4996) //Wyłączenie błędu w Visual Studio wynikające z nietrzymania się standardów przez Microsoft
	file = fopen(fileName, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		filesize = ftell(file);

		fseek(file, 0, SEEK_SET);
		result = new char[filesize + 1];
		int readsize = fread(result, 1, filesize, file);
		result[filesize] = 0;

		fclose(file);

		return result;
	}
	return NULL;
}

//Metoda wczytuje i kompiluje shader, a następnie zwraca jego uchwyt
GLuint ShaderProgram::loadShader(GLenum shaderType, const char* fileName)
{
	//Wygeneruj uchwyt na shader
	GLuint shader = glCreateShader(shaderType);//shaderType to GL_VERTEX_SHADER, GL_GEOMETRY_SHADER lub GL_FRAGMENT_SHADER
	//Wczytaj plik ze źródłem shadera do tablicy znaków
	const GLchar* shaderSource = readFile(fileName);
	//Powiąż źródło z uchwytem shadera
	glShaderSource(shader, 1, &shaderSource, NULL);
	//Skompiluj źródło
	glCompileShader(shader);
	//Usuń źródło shadera z pamięci (nie będzie już potrzebne)
	delete[]shaderSource;

	//Pobierz log błędów kompilacji i wyświetl
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	//Zwróć uchwyt wygenerowanego shadera
	return shader;
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile) {
	//Wczytaj vertex shader
	printf("Loading vertex shader...\n");
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFile);

	//Wczytaj geometry shader
	if (geometryShaderFile)
	{
		printf("Loading geometry shader...\n");
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryShaderFile);
	}
	else
		geometryShader = NULL;

	//Wczytaj fragment shader
	printf("Loading fragment shader...\n");
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	//Wygeneruj uchwyt programu cieniującego
	shaderProgram = glCreateProgram();

	//Podłącz do niego shadery i zlinkuj program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (geometryShaderFile) glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	//Pobierz log błędów linkowania i wyświetl
	int infologLength = 0,
		charsWritten = 0;
	char* infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram()
{
	//Odłącz shadery od programu
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	//Wykasuj shadery
	glDeleteShader(vertexShader);
	if (geometryShader) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Wykasuj program
	glDeleteProgram(shaderProgram);
}


//Włącz używanie programu cieniującego reprezentowanego przez aktualny obiekt
void ShaderProgram::use()
{
	glUseProgram(shaderProgram);
}

//Pobierz numer slotu odpowiadającego zmiennej jednorodnej o nazwie variableName
GLuint ShaderProgram::u(const char* variableName)
{
	return glGetUniformLocation(shaderProgram, variableName);
}

//Pobierz numer slotu odpowiadającego atrybutowi o nazwie variableName
GLuint ShaderProgram::a(const char* variableName)
{
	return glGetAttribLocation(shaderProgram, variableName);
}
