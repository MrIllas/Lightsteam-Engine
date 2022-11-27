#version 330 core

out vec4 FragColor;

in vec2 TextureCoords;

uniform sampler2D screenTexture

float near = 1.0f;
float far = 100.0f;
float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

void main()
{
	//FragColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.0f);
	 FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
} 




//vec3 objectColor = texture(uTexture, mobileTextureCoordinate).xyz;
//vec3 phong = (ambient + diffuse) * objectColor + specular;
//pyramidColor = vec4(phong, 1.0f);