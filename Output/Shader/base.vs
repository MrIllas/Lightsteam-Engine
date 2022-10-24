#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

//uniform vec3 lightPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; //Identidad


out vec2 TextureCoords;
out vec3 FragPos;
out vec3 Normal;
//out float TextureID;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	FragPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	
    TextureCoords = texCoord; 
    //TextureID = texID;
}