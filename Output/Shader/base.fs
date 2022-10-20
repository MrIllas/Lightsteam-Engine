#version 330 core

out vec4 FragColor;
  
in vec2 TextureCoords;
//in float TextureID;

//uniform sampler2D Textures[32];
uniform sampler2D texture_albedo;

void main()
{
    //FragColor = texture(Textures[int(TextureID)], TextureCoords);
    FragColor = texture(texture_albedo, TextureCoords); //vec4(0.0f, 1.0f, 0.0f, 1.0f);
} 