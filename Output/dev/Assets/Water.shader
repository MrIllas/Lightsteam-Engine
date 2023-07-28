#version 410 core
#ifdef VERTEX_PROGRAM

const float PI = 3.1415;

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model; 
uniform float LssTime;

uniform float timeMultiplier;
uniform sampler2D perlinNoiseMap; //Perlin texture
uniform sampler2D voronoiNoiseMap; //Voronoi texture

out vec2 TextureCoords;
out float waveHeight;

struct Wave {
    vec2 direction;
    float steepness;
    float waveLength;
};

// Gerstner Wave calculation.
vec3 GerstnerWave(vec3 p, Wave wave, float time) {
    float steepness = wave.steepness;
    float wavelength = wave.waveLength;

    float k = 2.0 * PI / wave.waveLength;
    float c = sqrt(9.8 / k);
    vec2 d = normalize(wave.direction);
    float f = k * (dot(d, p.xy) - c * time);
    float a = wave.steepness / k;

    return vec3(
        d.x * (a * cos(f)),
        a * sin(f),
        d.y * (a * cos(f))
    );
}

float turbulance(sampler2D map, float t)
{
	vec2 offset1 = vec2(0.8, 0.4) * t/10.0f;
	vec2 offset2 = vec2(0.6, 1.1) * t/10.0f;
	float height1 = texture2D(map, texCoord + offset1).r * 0.05;
	float height2 = texture2D(map, texCoord + offset2).r * 0.05;
	
	return height1 + height2;
}

vec3 displacement(vec3 point, Wave[4] wave, float t) {

  vec3 p = point;
  vec3 n = vec3(0.0f, 0.0f, 0.0f);
  
  n.z += turbulance(perlinNoiseMap, t * -0.5);
  n.z -= turbulance(voronoiNoiseMap, t * 0.5);
  
  //Requires z and y position swap
  n += GerstnerWave(p, wave[0], t).xzy;
  n += GerstnerWave(p, wave[1], t).xzy * 0.5;
  n += GerstnerWave(p, wave[2], t).xzy * 0.25;
  n += GerstnerWave(p, wave[3], t).xzy * 0.2;
  
  waveHeight = n.z;
  
  return point + n;
}

void main()
{	
	Wave[4] waves;
	waves[0] = Wave(vec2(0.0, -1.0), 0.3, 0.5);
	waves[1] = Wave(vec2(0.0, 1.0), 0.10, 1.5);
	waves[2] = Wave(vec2(1.0, 1.0), 0.05, 2.7);
	waves[3] = Wave(vec2(1.0, 1.0), 0.2, 0.5);

	vec3 pos = displacement(aPos, waves, LssTime * timeMultiplier);
	
	
	gl_Position = Projection * View * Model* vec4(pos, 1.0f);
	TextureCoords = texCoord;
}

#endif
#ifdef FRAGMENT_PROGRAM
in vec2 TextureCoords;
in float waveHeight;

out vec4 FragColor;

uniform vec3 baseColor;
uniform vec3 foamColor;
uniform float offset;
uniform float contrast;
uniform float brightness;

void main()
{
	float maskFoam = (waveHeight - offset) * contrast;

	vec3 colorDiffuse = mix(baseColor, foamColor, maskFoam);
	colorDiffuse *= brightness;
	
	FragColor = vec4(colorDiffuse, 1.0f);
} 

#endif

