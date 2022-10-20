#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; //Identidad

out vec2 TextureCoords;
//out float TextureID;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TextureCoords = texCoord; 
    //TextureID = texID;
}