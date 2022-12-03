#include "TextureImporter.h"

#include "LibraryManager.h"

#include "Glew/include/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

#include "Nlohmann/json.hpp"

#include "MeshRenderer.h"

#include "ResourceTexture.h"

Texture TextureImporter::checkers;

TextureImporter::TextureImporter()
{
	ilInit();
}

TextureImporter::~TextureImporter()
{
	
}

void TextureImporter::CheckerImage()
{
	const uint checkersHeight = 255;
	const uint checkersWidth = 255;

	GLubyte checkerImage[checkersHeight][checkersWidth][4];
	for (int i = 0; i < checkersHeight; i++) {
		for (int j = 0; j < checkersWidth; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkers.id);
	glBindTexture(GL_TEXTURE_2D, checkers.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkersWidth, checkersHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	checkers.path = "DEBUG TEXTURE(CHECKERS)";
	checkers.w = checkersWidth;
	checkers.h = checkersHeight;
}


TextureData* TextureImporter::ImportTexture(std::string filePath)
{
	Texture auxText;
	/*int checkTxt = CheckTexturesLoaded(filePath, auxText);
	if (checkTxt > -1)
	{
		LOG(LOG_TYPE::ATTENTION, "ATTENTION: The image '%s' was already imported, loading it from memory.", filePath.c_str());
		return auxText;
	}*/

	ILuint imgID = 0;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ILboolean success = ilLoadImage(filePath.c_str());

	if (success == IL_TRUE)
	{
		uint w = ilGetInteger(IL_IMAGE_WIDTH);
		uint h = ilGetInteger(IL_IMAGE_HEIGHT);
		LOG(LOG_TYPE::SUCCESS, "SUCCESS: Loading image '%s' of size (%i x %i) pixels .", filePath.c_str(), w, h);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		
		TextureData* txtData = new TextureData();;

		glGenTextures(1, &txtData->texture.id);
		glBindTexture(GL_TEXTURE_2D, txtData->texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		txtData->data = ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, IL_RGBA, w, h, 0, IL_RGBA, GL_UNSIGNED_BYTE, txtData->data);

		ilDeleteImages(1, &imgID);
		glBindTexture(GL_TEXTURE_2D, 0);

		txtData->texture.w = w;
		txtData->texture.h = h;
		txtData->texture.path = filePath;

		if (checkers.id == 0) CheckerImage();

		return txtData;
	}
	else
	{
		LOG(LOG_TYPE::ERRO, "ERROR: Could not load Texture '%s'", ilutGetString(ilGetError()));
	}
	
	//return checkers;
	return nullptr;
}

Texture TextureImporter::ImportFromLibrary(ResourceTexture* resource)
{
	if (resource == nullptr) return Texture();

	//Increase Reference Count
	resource->IncreaseRC();

	TextureData* toReturn = nullptr;
	//Check if it has been already loaded
	if (resource->GetRC() > 1)
	{
		LOG(LOG_TYPE::SUCCESS, "RC '%i': Texture '%s' is already loaded.", resource->GetRC(), resource->GetLibraryFile().c_str());
		toReturn = resource->texture;
	}
	else
	{
		//LOG(LOG_TYPE::ATTENTION, "Loading new texture to memory.");
		resource->texture = ImportTexture(resource->GetLibraryFile());
		resource->texture->texture.resUuid = resource->GetUUID(); //Assign the res uuid to the texture.
		toReturn = resource->texture;
	}
	
	if (toReturn == nullptr) return Texture();
	else return toReturn->texture;
}

//Loads assets image to memory, converts it to dds and saves it to Library
void TextureImporter::ImportToLibrary(ResourceTexture* resource)
{
	std::string path = LIB_TEXTURE;

	//Transform to Library path
	////Gives new path
	path += resource->GetUUID();
	path += ".dds";

	resource->SetLibraryFile(path);

	ILuint imgID = 0;

	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	ILboolean success = ilLoadImage(resource->GetAssetsFile().c_str());

	if (success == IL_TRUE)
	{
		uint w = ilGetInteger(IL_IMAGE_WIDTH);
		uint h = ilGetInteger(IL_IMAGE_HEIGHT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		BYTE* data = nullptr;

		glGenTextures(1, &imgID);
		glBindTexture(GL_TEXTURE_2D, imgID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		data = ilGetData();
		glTexImage2D(GL_TEXTURE_2D, 0, IL_RGBA, w, h, 0, IL_RGBA, GL_UNSIGNED_BYTE, data);

		//Load to dds
		ILuint size = 0;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
		size = ilSaveL(IL_DDS, nullptr, 0);

		if (size > 0) {
			data = new ILubyte[size]; 
			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				LibraryManager::Save(resource->GetLibraryFile(), (char*)data, size);
			}
		}
		else
		{
			LOG(LOG_TYPE::ERRO, "ERROR: Could not save image to library '%s'", ilutGetString(ilGetError()));
		}

		RELEASE_ARRAY(data);
		ilDeleteImages(1, &imgID);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		LOG(LOG_TYPE::ERRO, "ERROR: Could not save image to library '%s'", ilutGetString(ilGetError()));
	}
}