#version 330 core

#define POINT_LIGHTS 4
#define SPOT_LIGHTS 1

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float empower;
	float shininess;	
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 falloff; // (Constant, Linear, Quadratic)
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec2 cutoff; // (inner radius, outer radius)
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 position, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 position, vec3 normal, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);

	vec3 cumulative = CalcDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < POINT_LIGHTS; ++i){
		cumulative += CalcPointLight(pointLights[i], FragPos, normal, viewDir);
	}
	for(int i = 0; i < SPOT_LIGHTS; ++i){
		cumulative += CalcSpotLight(spotLights[i], FragPos, normal, viewDir);
	}
	vec3 emission = vec3(texture(material.emission, TexCoords)) * material.empower;
	color = vec4(cumulative + emission, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(-lightDir, normal);

	float diff = max(dot(normal, lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 position, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(light.position - position);
	vec3 reflectDir = reflect(-lightDir, normal);

	float diff = max(dot(normal, lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	float distance = length(light.position - position);
	float attenuation = 1.0f / (light.falloff.x + light.falloff.y * distance + light.falloff.z * distance * distance);

	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 position, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(light.position - position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff.x - light.cutoff.y;
	float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(normal, lightDir), 0.0f);
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0f), material.shininess);
	
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (diffuse + specular) * intensity;
}