#include "ObjMtlParser.h"
#include <vector>
#include <string>
#include "lodepng.h"
#include <assimp/scene.h>



bool ObjMtlParser::loadOBJ(
	const char* path,
	std::vector < glm::vec4 >& out_vertices,
	std::vector < glm::vec2 >& out_uvs,
	std::vector < glm::vec4 >& out_normals
)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec4 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec4 > temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec4 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.w = 1;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec4 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.w = 0;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec4 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	// For each uv of each triangle
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	// For each normal of each triangle
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec4 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}

GLuint ObjMtlParser::readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}
	
LoadedObjModel::LoadedObjModel() {}

	
LoadedObjModel::LoadedObjModel(float* vertices, float* normals, float* texCords, int verticesCount, GLuint* tex_ptr , GLuint* texspec_ptr)
{
	_vertices = vertices;
	_normals = normals;
	_texCoords = texCords;
	_verticesCount = verticesCount;
	_tex = tex_ptr;
	_texspec = texspec_ptr;
}

LoadedObjModel::LoadedObjModel(const char* model_path, GLuint* tex_ptr, GLuint* texspec_ptr)
{
	//model loading
	if (!ObjMtlParser::loadOBJ(model_path, vecVertices, vecTexCoords, vecNormals))
	{
		printf("Model not loaded: %s\n", model_path);
	}//procedura bledu TODO
	else
	{
		printf("Model loaded: %s\n", model_path);
		_vertices = &vecVertices[0].x;
		_normals = &vecNormals[0].x;
		_texCoords = &vecTexCoords[0].x;
		_verticesCount = vecVertices.size();
	}	_tex = tex_ptr;
	_texspec = texspec_ptr;	
}

LoadedObjModel::LoadedObjModel(const aiScene* sc)
{
	for (int i = 0; i < sc->mMeshes[0]->mNumVertices; i++)
	{
		vecVertices.push_back(glm::vec4(sc->mMeshes[0]->mVertices[i].x, sc->mMeshes[0]->mVertices[i].y, sc->mMeshes[0]->mVertices[i].z, 1));
		//printf("Vertex %d: %f, %f, %f\n",i, sc->mMeshes[0]->mVertices[i].x, sc->mMeshes[0]->mVertices[i].y, sc->mMeshes[0]->mVertices[i].z);
	}
	for (int i = 0; i < sc->mMeshes[0]->mNumVertices; i++)
	{
		vecNormals.push_back(glm::vec4(sc->mMeshes[0]->mNormals[i].x, sc->mMeshes[0]->mNormals[i].y, sc->mMeshes[0]->mNormals[i].z, 0));
		//printf("Normals %d: %f, %f, %f\n", i, sc->mMeshes[0]->mNormals[i].x, sc->mMeshes[0]->mNormals[i].y, sc->mMeshes[0]->mNormals[i].z);
	}
	for (int i = 0; i < sc->mMeshes[0]->mNumVertices; i++)
		vecTexCoords.push_back(glm::vec2(sc->mMeshes[0]->mTextureCoords[0][i].x, sc->mMeshes[0]->mTextureCoords[0][i].y));
	//for (int i = 0; i < sc->mMeshes[0]->mNumVertices; i++)
	//	printf("TexCord %d : %f", i, sc->mMeshes[0]->mTextureCoords[i]);
	printf("Loaded\n");
	_vertices = &vecVertices[0].x;
	_normals = &vecNormals[0].x;
	_texCoords = &vecTexCoords[0].x;
	_verticesCount = vecVertices.size();
}

GLuint LoadedObjModel::tex()
{		
	return *_tex;
}

GLuint LoadedObjModel::tex_spec()
{
	return *_texspec;
}

void LoadedObjModel::setTex(GLuint* tex_ptr)
{
	_tex = tex_ptr;
}

void LoadedObjModel::setTex_spec(GLuint* texspec_ptr)
{
	_texspec = texspec_ptr;
}

void LoadedObjModel::setColors(float* colors)
{
	_colors = colors;
}

bool DoTheImportThing(const std::string& pFile, LoadedObjModel* model) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile, 0);

	if (!scene) {
		return false;
	}

	printf("Vertexes: %d\n", scene->mMeshes[0]->mNumVertices);
	*model = LoadedObjModel(scene);
	printf("Loaded ASS: %s\n", &pFile);
	return true;
}