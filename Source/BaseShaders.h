#pragma once

#include <string>

namespace InEngineShaders
{
#pragma region BaseShader
	std::string baseFragment = R"(
		#version 410 core

		out vec4 FragColor;
  
		in vec2 TextureCoords;
		uniform sampler2D texture_albedo;

		void main()
		{
			FragColor = texture(texture_albedo, TextureCoords);
		} 
	)";

	std::string baseVertex = R"(
		#version 410 core

		layout (location = 0) in vec3 aPos; 
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoord;

		uniform mat4 projection;
		uniform mat4 view;
		uniform mat4 model; 

		out vec2 TextureCoords;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
			TextureCoords = texCoord; 
		}
	)";
#pragma endregion Base Shader

#pragma region Debug Shader
	std::string debugFragment = R"(
	#version 410 core

	out vec4 FragColor;
  
  
	void main()
	{
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	)";

	std::string debugVertex = R"(
	#version 410 core

	layout (location = 0) in vec3 aPos; 

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model; 

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	}
	)";
#pragma endregion Debug Shader

}