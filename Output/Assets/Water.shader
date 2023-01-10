#version 410 core
#ifdef VERTEX_PROGRAM

#define NUM_CELLS 20

vec2 gerstnerWave(in float t, in vec2 p, in vec2 dir, in float A, in float w, in float phi)
{
    // Compute the wave displacement
    float disp = A * sin(dot(dir, p) - w * t + phi);
    float disp2 = A * cos(dot(-dir, p) - w * t + phi);

    // Perturb the vertex position
    p.x += disp * dir.y;
    p.y -= disp2 * dir.x;

    return p;
}

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model; 

uniform float LssTime;

uniform float A;
uniform float w;
uniform float phi;

uniform float A2;
uniform float w2;
uniform float phi2;


out vec2 TextureCoords;


void main()
{
	// Get the vertex position and normal
    vec3 pos = aPos;
    vec3 norm = normal;

    // Compute the time
    float t = LssTime;

    // Set the wave parameters
    vec2 dir = vec2(norm.z, norm.z);

    // Generate the Gerstner wave
    pos.xz = gerstnerWave(t, pos.xz, dir, A, w, phi) + gerstnerWave(-t, pos.xz, dir, A2, w2, phi2);
	
	vec4 mvPosition = View * Model* vec4(pos, 1.0f);	
	
	gl_Position = Projection * View * Model* vec4(pos, 1.0f);
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













