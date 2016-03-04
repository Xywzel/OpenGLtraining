#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;	
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 falloff; // (Constant, Linear, Quadratic)
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec2 cutoff; // (inner radius, outer radius)
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform SpotLight light;
uniform vec3 viewPos;

void main()
{
	//float distance = length(light.position - FragPos);
	//float attenuation = 1.0f / (light.falloff.x + light.falloff.y * distance + light.falloff.z * distance * distance);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	// vec3 lightDir = normalize(-light.direction);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff.x - light.cutoff.y;
	float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0f, 1.0f);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	vec3 emission = vec3(texture(material.emission, TexCoords)) * 0.01f;

	color = vec4(ambient + (diffuse + specular)*intensity + emission, 1.0f);
}