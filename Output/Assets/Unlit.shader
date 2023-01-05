#version 410 core
#ifdef VERTEX_PROGRAM

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model; 

out vec2 TextureCoords;


void main()
{
	gl_Position = Projection * View * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	TextureCoords = texCoord; 
}

#endif
#ifdef FRAGMENT_PROGRAM

uniform sampler2D texture_albedo;

in vec2 TextureCoords;

out vec4 FragColor;
uniform vec3 vColour;

void main()
{
	FragColor = texture(texture_albedo, TextureCoords) * vec4(vColour, 1.0f);
} 

#endif



