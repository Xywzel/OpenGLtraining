#version 330 core

in vec2 TexCoords;

out vec4 color;

struct Material {
	sampler2D diffuse0;
	sampler2D diffuse1;
	sampler2D specular0;
	sampler2D specular1;
	sampler2D emission0;
	sampler2D emission1;
	float empower;
	float shininess;	
};

uniform Material material;

void main()
{    
    color = vec4(texture(material.diffuse0, TexCoords));
}