#include "ObjMtlParser.h"
#include <vector>
#include <string>
#include "lodepng.h"
#include <assimp/scene.h>



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

LoadedObjModel::~LoadedObjModel() 
{
	if (!_dataPointedOutside)
	{
		if (_vertices!=NULL)
		delete[] _vertices;
		if (_normals != NULL)
		delete[] _normals;
		if (_texCoords != NULL)
		delete[] _texCoords;
		//if (_colors != NULL)
		delete[] _colors;
		if (_indexes != NULL)
		delete[] _indexes;
	}

	this->delete_tex();
	this->delete_texspec();

}

LoadedObjModel::LoadedObjModel(float* vertices, float* normals, float* texCords, int verticesCount, GLuint* tex_ptr , GLuint* texspec_ptr)
{
	_dataPointedOutside = true;
	_vertices = vertices;
	_normals = normals;
	_texCoords = texCords;
	_verticesCount = verticesCount;
	_tex_ptr = tex_ptr;
	_texspec_ptr = texspec_ptr;
}

LoadedObjModel::LoadedObjModel(const aiScene* sc, int id)
{
	_dataPointedOutside = false;
	printf("Name: %s\n", sc->mMeshes[id]->mName.C_Str());
	printf("Meshes: %d\n", sc->mNumMeshes);
	printf("Vertexes: %d\n", sc->mMeshes[id]->mNumVertices);
	_vertices = new float[4*sc->mMeshes[id]->mNumVertices];
	for (int i = 0; i < sc->mMeshes[id]->mNumVertices; i++)
	{
		_vertices[i * 4] = sc->mMeshes[id]->mVertices[i].x;
		_vertices[i * 4+1] = sc->mMeshes[id]->mVertices[i].y;
		_vertices[i * 4+2] = sc->mMeshes[id]->mVertices[i].z;
		_vertices[i * 4 + 3] = 1;
	}

	_normals = new float[4 * sc->mMeshes[id]->mNumVertices];
	for (int i = 0; i < sc->mMeshes[id]->mNumVertices; i++)
	{
		_normals[i * 4] = sc->mMeshes[id]->mNormals[i].x;
		_normals[i * 4 + 1] = sc->mMeshes[id]->mNormals[i].y;
		_normals[i * 4 + 2] = sc->mMeshes[id]->mNormals[i].z;
		_normals[i * 4 + 3] = 0;
	}
	printf("mNumFaces: %d\n", sc->mMeshes[id]->mNumFaces);
	_indexes = new unsigned int[3 * sc->mMeshes[id]->mNumFaces];
	for (int i = 0; i < sc->mMeshes[id]->mNumFaces; i++)
	{
		_indexes[i * 3]=sc->mMeshes[id]->mFaces[i].mIndices[0];
		_indexes[i * 3 + 1] = sc->mMeshes[id]->mFaces[i].mIndices[1];
		_indexes[i * 3 + 2] = sc->mMeshes[id]->mFaces[i].mIndices[2];
	}
	hasIndexes = 3 * sc->mMeshes[id]->mNumFaces;

	for (int i = 0 ; i < 8 ; i++)printf("Texture Set %d: %s\n",i, sc->mMeshes[id]->HasTextureCoords(i)? "true" : "false");
	
	if (sc->mMeshes[id]->HasTextureCoords(0))
	{	
		_texCoords = new float[2 * 3 * sc->mMeshes[id]->mNumFaces];
		for (int i = 0; i < 3 * sc->mMeshes[id]->mNumFaces; i++)
		{
			_texCoords[i * 2] = sc->mMeshes[id]->mTextureCoords[0][i].x;
			_texCoords[i * 2 + 1] = sc->mMeshes[id]->mTextureCoords[0][i].y;
		}
	}
	if (sc->HasTextures())
	{
		glGenTextures(1, &_tex); //Zainicjuj jeden uchwyt
		glBindTexture(GL_TEXTURE_2D, _tex); //Uaktywnij uchwyt
		glTexImage2D(GL_TEXTURE_2D, 0, 4, sc->mTextures[0]->mWidth, sc->mTextures[0]->mHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)sc->mTextures[0]->pcData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		printf("texture loaded\n");
	}

	printf("Primitives: %d\n", sc->mMeshes[id]->mPrimitiveTypes);
	_verticesCount = sc->mMeshes[id]->mNumVertices;
	printf("Loaded\n");
}

GLuint LoadedObjModel::tex()
{		
	return *_tex_ptr;
}

GLuint LoadedObjModel::tex_spec()
{
	return *_texspec_ptr;
}

void LoadedObjModel::setTexPtr(GLuint* tex_ptr)
{
	_tex_ptr = tex_ptr;
}

void LoadedObjModel::setTex_specPtr(GLuint* texspec_ptr)
{
	_texspec_ptr = texspec_ptr;
}

void LoadedObjModel::setTex(GLuint tex)
{
	_tex = tex;
}


void LoadedObjModel::setTex_spec(GLuint texspec)
{
	_texspec = texspec;
}

void LoadedObjModel::loadTex(const char* filename)
{
	_tex = ObjMtlParser::readTexture(filename);
	_tex_ptr = &_tex;
}

void LoadedObjModel::loadTex_spec(const char* filename)
{
	_texspec = ObjMtlParser::readTexture(filename);
	_texspec_ptr = &_texspec;
}


void LoadedObjModel::delete_tex()
{
	glDeleteTextures(1, &_tex);
}
void LoadedObjModel::delete_texspec()
{
	glDeleteTextures(1, &_texspec);
}

void LoadedObjModel::setColors(float* colors)
{
	_colors = colors;
}

bool loadObj(const std::string& pFile, LoadedObjModel** model,int id, int aibit) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile, aiProcess_Triangulate| aiProcess_FlipUVs);

	if (!scene) {
		return false;
	}
	*model = new LoadedObjModel(scene, id);
	printf("Loaded ASS: %s\n\n", &pFile);
	return true;
}

bool loadMultipleObj(const std::string& pFile, LoadedObjModel*** model, int n, int aibit) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene) {
		return false;
	}
	for(int i = 0 ; i < n ; i++)
		*model[i] = new LoadedObjModel(scene, i);
	printf("Loaded ASS: %s\n\n", &pFile);
	return true;
}

bool loadAnimationObj(const std::string* pFile, LoadedObjModel** model, int n, int aibit) {
	Assimp::Importer importer;

	for(int i = 0 ; i < n ; i ++)
	{
		printf("Loading ASS: %s\n", pFile[i]);
		const aiScene* scene = importer.ReadFile(pFile[i], aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene) {
			return false;
		}
		model[i] = new LoadedObjModel(scene, 0);
		printf("Loaded ASS: %s\n\n", pFile[i]);
	}
	return true;
}