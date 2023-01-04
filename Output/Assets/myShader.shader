#version 410 core
#ifdef VERTEX_PROGRAM

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; 

//uniform int testInt;
//uniform uint testUInt;
//uniform float testFloat;
uniform mat4 testDouble;
uniform bool testBool;


out vec2 TextureCoords;

void main()
{
	if (testBool)
	{
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, testDouble[0][0]);
		TextureCoords = texCoord; 
	}
	
}

#endif

#ifdef FRAGMENT_PROGRAM

out vec4 FragColor;
in vec2 TextureCoords;
uniform sampler2D texture_albedo;

void main()
{
	FragColor = texture(texture_albedo, TextureCoords);
} 

#endif


























