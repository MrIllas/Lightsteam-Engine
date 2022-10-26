#version 330 core

out vec4 FragColor;
  
in vec2 TextureCoords;
in vec3 FragPos; 
in vec3 Normal;
//in vec3 lightPos;
//in vec3 lightColour;
//in float specularStrength

uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float specularStrength;
//in float TextureID;

//uniform sampler2D Textures[32];
uniform sampler2D texture_albedo;

void main()
{
	//vec3 lightPos = vec3(5.0f, 5.0f, 0.0f);
	//vec3 lightColour = vec3(0.0f, 1.0f, 0.0f);
	vec3 viewPos = vec3(-5.0f, -5.0f, 0.0f);
	//float specularStrength = 0.5;
	
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
   // FragColor = ambient * texture(texture_albedo, TextureCoords); //vec4(0.0f, 1.0f, 0.0f, 1.0f);
	FragColor = vec4(phong, 1.0f);
} 

//vec3 objectColor = texture(uTexture, mobileTextureCoordinate).xyz;
//vec3 phong = (ambient + diffuse) * objectColor + specular;
//pyramidColor = vec4(phong, 1.0f);