#include "modelParser.h"

GLuint readTexture(const char* filename)
{
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filename);
	if (error)
		std::cerr << "Nie udalo sie wczytac tekstury\n";

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

LoadedModel::~LoadedModel()
{
	if (!dataPointedOutside)
	{
		if (vertices)
			delete[] vertices;

		if (normals)
			delete[] normals;

		if (texCoords)
			delete[] texCoords;

		if (colors)
			delete[] colors;

		if (indexes)
			delete[] indexes;
	}
	glDeleteTextures(1, &tex);
	glDeleteTextures(1, &texSpec);
}

LoadedModel::LoadedModel(const aiScene* sc, int id)
{
	dataPointedOutside = false;

	std::cout << "Name: "		<<	sc->mMeshes[id]->mName.C_Str()	<< std::endl;
	std::cout << "Meshes: "		<<	sc->mNumMeshes					<< std::endl;
	std::cout << "Vertexes: "	<<	sc->mMeshes[id]->mNumVertices	<< std::endl;

	vertices = new float[4 * sc->mMeshes[id]->mNumVertices];
	for (int i = 0, index = 0; i != sc->mMeshes[id]->mNumVertices; ++i)
	{
		vertices[index++]	= sc->mMeshes[id]->mVertices[i].x;
		vertices[index++]	= sc->mMeshes[id]->mVertices[i].y;
		vertices[index++]	= sc->mMeshes[id]->mVertices[i].z;
		vertices[index++]	= 1;
	}

	normals = new float[4 * sc->mMeshes[id]->mNumVertices];
	for (unsigned i = 0, index = 0; i != sc->mMeshes[id]->mNumVertices; ++i)
	{
		normals[index++]	= sc->mMeshes[id]->mNormals[i].x;
		normals[index++]	= sc->mMeshes[id]->mNormals[i].y;
		normals[index++]	= sc->mMeshes[id]->mNormals[i].z;
		normals[index++]	= 0;
	}

	std::cout << "mNumFaces: " << sc->mMeshes[id]->mNumFaces << std::endl;
	indexes = new unsigned int[3 * sc->mMeshes[id]->mNumFaces];
	for (unsigned i = 0, index = 0; i != sc->mMeshes[id]->mNumFaces; ++i)
	{
		indexes[index++]	= sc->mMeshes[id]->mFaces[i].mIndices[0];
		indexes[index++]	= sc->mMeshes[id]->mFaces[i].mIndices[1];
		indexes[index++]	= sc->mMeshes[id]->mFaces[i].mIndices[2];
	}
	hasIndexes = 3 * sc->mMeshes[id]->mNumFaces;

	for (unsigned i = 0 ; i != 8 ; ++i)
		std::cout << "Texture Set " << i << ':' << (sc->mMeshes[id]->HasTextureCoords(i)? "true" : "false");
	
	if (sc->mMeshes[id]->HasTextureCoords(0))
	{	
		unsigned faces = 3 * sc->mMeshes[id]->mNumFaces;
		texCoords = new float[6 * sc->mMeshes[id]->mNumFaces];
		for (unsigned i = 0; i != 3 * sc->mMeshes[id]->mNumFaces; i++)
		{
			texCoords[i * 2] = sc->mMeshes[id]->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = sc->mMeshes[id]->mTextureCoords[0][i].y;
		}
	}

	if (sc->HasTextures())
	{
		glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
		glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
		glTexImage2D(GL_TEXTURE_2D, 0, 4, sc->mTextures[0]->mWidth, sc->mTextures[0]->mHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)sc->mTextures[0]->pcData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout << "texture loaded\n";
	}

	std::cout << "Primitives: \n" << sc->mMeshes[id]->mPrimitiveTypes;
	verticesCount = sc->mMeshes[id]->mNumVertices;
	std::cout << "Loaded\n";
}

bool LoadedModel::loadObj(const std::string& pFile, LoadedModel* &model, int id)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene) 
		return false;

	model = new LoadedModel(scene, id);
	std::cout << "Loaded model:" << pFile << "\n\n";
	return true;
}