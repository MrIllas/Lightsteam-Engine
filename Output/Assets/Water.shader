#version 410 core
#ifdef VERTEX_PROGRAM

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model; 

uniform sampler2D heightMap;
uniform float LssTime;


out vec2 TextureCoords;


void main()
{
	vec3 pos = aPos;
	vec2 offset1 = vec2(0.8, 0.4) * LssTime / 10.0f;
	vec2 offset2 = vec2(0.6, 1.1) * LssTime / 10.0f;
	float height1 = texture2D(heightMap, texCoord + offset1).r * 0.05;
	float height2 = texture2D(heightMap, texCoord + offset2).r * 0.05;
	
	pos.z += height1 + height2;
	vec4 mvPosition = View * Model* vec4(pos, 1.0f);
	
	
	
	gl_Position = Projection * mvPosition;
	TextureCoords = texCoord;
}

#endif
#ifdef FRAGMENT_PROGRAM

uniform sampler2D texture_albedo;

in vec2 TextureCoords;
uniform vec3 baseColor;
out vec4 FragColor;

void main()
{
	vec4 c = vec4(baseColor.xyz, 1.0f);
	FragColor = texture(texture_albedo, TextureCoords) * vec4(baseColor, 1.0f);
} 

#endif











