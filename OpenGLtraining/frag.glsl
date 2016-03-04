#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float ambient = 0.1f;
	vec3 diffuse = max(dot(norm, lightDir), 0.0f) * lightColor;
	color = vec4((ambient + diffuse) * lightColor * objectColor, 1.0f);
}