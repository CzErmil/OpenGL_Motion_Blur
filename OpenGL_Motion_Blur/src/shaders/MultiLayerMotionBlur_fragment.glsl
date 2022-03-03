#version 330 core

in vec3 gs_normal;
in vec2 gs_textureCoord;
in vec3 gs_fragCoord;

uniform sampler2D u_texture;
uniform vec3 u_CameraPos;
// uniform PointLight pointLights[2];

out vec4 color;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

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

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), 16);

	float d = distance(light.position, fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

void main(void)
{
	vec3 normal = normalize(gs_normal);
	
	vec3 viewDir = normalize(u_CameraPos - gs_fragCoord);

	PointLight light[2];
	light[0].position	= vec3(0,4,0);
	light[1].position	= vec3(5,0,0);
	light[0].constant	=light[1].constant	= 1;
	light[0].linear		=light[1].linear	= 0.5;
	light[0].quadratic	=light[1].quadratic	= 0.032;
	light[0].ambient	=light[1].ambient	= vec3(1.0f, 1.0f, 1.0f);
	light[0].diffuse	=light[1].diffuse	= vec3(0.8f, 0.8f, 0.8f);
	light[0].specular	=light[1].specular	= vec3(0.5f, 0.5f, 0.5f);

	vec3 result = vec3(0,0,0);
	for(int i =0; i<2; i++)
		result += calcPointLight(light[i], normal, gs_fragCoord,  viewDir);

	color = texture(u_texture, gs_textureCoord) * vec4(result, 1);
}