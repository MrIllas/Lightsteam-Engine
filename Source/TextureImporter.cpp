#include "TextureImporter.h"


#include "Glew/include/glew.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

#include "MeshRenderer.h"

std::vector<TextureData*> TextureImporter::texturesLoaded;

Texture TextureImporter::checkers;

TextureImporter::TextureImporter()
{
	ilInit();
}

TextureImporter::~TextureImporter()
{
	for (int i = 0; i < texturesLoaded.size(); ++i)
	{
		RELEASE(texturesLoaded[i]);
	}
	texturesLoaded.clear();
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


Texture TextureImporter::ImportTexture(std::string filePath)
{
	Texture auxText;
	int checkTxt = CheckTexturesLoaded(filePath, auxText);
	if (checkTxt > -1)
	{
		LOG(LOG_TYPE::ATTENTION, "ATTENTION: The image '%s' was already imported, loading it from memory.", filePath.c_str());
		return auxText;
	}

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
		texturesLoaded.emplace_back(txtData);

		if (checkers.id == 0) CheckerImage();

		return txtData->texture;
	}
	else
	{
		LOG(LOG_TYPE::ERRO, "ERROR: Could not load Texture '%s'", ilutGetString(ilGetError()));
	}
	
	return checkers;
}

int TextureImporter::CheckTexturesLoaded(std::string filePath, Texture& texture)
{
	for (int i = 0; i < texturesLoaded.size(); ++i)
	{
		if (!texturesLoaded[i]->texture.path.compare(filePath))
		{
			texture = texturesLoaded[i]->texture;
			return texturesLoaded[i]->texture.id;
		}
	}

	return -1;
}