#version 410 core

#ifdef FRAGMENT_PROGRAM

out vec4 FragColor;
  
in vec2 TextureCoords;
in vec3 FragPos; 
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float specularStrength;

uniform sampler2D texture_albedo;

float near = 1.0f;
float far = 100.0f;
float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

void main()
{
	vec3 viewPos = vec3(-5.0f, -5.0f, 0.0f);
	
	//Ambient light 
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;
	
	//Difuse light
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour;
	
	//Specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;
	
	vec3 objectColour = texture(texture_albedo, TextureCoords).xyz;
	
	vec3 phong = (ambient + diffuse) * objectColour;

	FragColor = vec4(phong, 1.0f);
	
} 

#endif

#ifdef VERTEX_PROGRAM

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model; 


out vec2 TextureCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	FragPos = vec3(model * vec4(aPos.x, aPos.y, aPos.z, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	
    TextureCoords = texCoord; 
}

#endif