#version 330 core

in vec3 gs_normal;
in vec2 gs_textureCoord;
in vec3 gs_fragCoord;

uniform sampler2D u_texture;
uniform vec3 u_CameraPos;

out vec4 color;

struct DirLight
{
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform DirLight light;
uniform Material material;

vec3 calcPointLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(u_texture, gs_textureCoord));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(u_texture, gs_textureCoord)));
	vec3 specular = light.specular * (spec * material.specular);

	return (ambient + diffuse + specular);
}

void main(void)
{
	vec3 normal = normalize(gs_normal);
	
	vec3 viewDir = normalize(u_CameraPos - gs_fragCoord);

	vec3 result = vec3(0,0,0);

	result += calcPointLight(light, normal, gs_fragCoord,  viewDir);

	color = vec4(result, 1);
}