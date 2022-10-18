#include "TextureImporter.h"

#include"Globals.h"

#include "Glew/include/glew.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

#include "MeshRenderer.h"

TextureImporter::TextureImporter()
{

}

TextureImporter::~TextureImporter()
{

}

uint TextureImporter::CheckerImage()
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

	uint textureID = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkersWidth, checkersHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);

	return textureID;
}


uint TextureImporter::ImportTexture(std::string filePath)
{
	ILuint imgID = 0;
	GLuint texture = ilutGLBindTexImage();

	ilGenImages(1, &imgID);
	ilBindImage(imgID);

	if (!ilLoadImage(filePath.c_str()))
	{
		LOG("ERROR: Could not load Texture '%s'", ilutGetString(ilGetError()));
	}
	

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imgID);

	return texture;
}