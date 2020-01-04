#version 330 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

// 物体材质
struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;

	float ambientCoe;
	float shinness;
};
uniform Material material;

// 方向光
struct DirectLight
{
	vec3 direction;
	vec3 color;
};
uniform DirectLight dirLight;

// 点光源
struct PointLight
{
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};
#define POINT_LIGHT_NUM 4
uniform PointLight pointLights[POINT_LIGHT_NUM];

// 聚光灯
struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;
};
uniform SpotLight spotLight;

vec3 calDirectionLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness);
vec3 calPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness);
vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness);

uniform vec3 viewPos;

out vec4 FragColor;
void main()
{
	vec3 norm = normalize(normal);
	vec3 texDiffuse = texture(material.diffuse1, texCoord).rgb;
	vec3 texSpecular = texture(material.specular1, texCoord).rgb;
	vec3 texAmbient = texDiffuse * material.ambientCoe;
	vec3 viewDir = normalize(viewPos - fragPos);

	// 方向光
	vec3 outColor = calDirectionLight(dirLight, norm, viewDir, texDiffuse, texAmbient, texSpecular, material.shinness);
	// 点光源
	for(int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		outColor += calPointLight(pointLights[i], norm, fragPos, viewDir, texDiffuse, texAmbient, texSpecular, material.shinness);
	}
	// 聚光灯
	outColor += calSpotLight(spotLight, norm, fragPos, texDiffuse, texAmbient, texSpecular, material.shinness);

	FragColor = vec4(outColor, 1.0);
//	FragColor = texture(material.diffuse1, texCoord);
}

vec3 calDirectionLight(DirectLight light, vec3 normal, vec3 viewDir, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness)
{
	// diffuse color
	vec3 frag2Light_norm = normalize(-light.direction);
	vec3 diffuse = max(dot(frag2Light_norm, normal), 0.0) * objDiffuse * light.color;

	// ambient color
	vec3 ambient = objAmbient * spotLight.color * 0.9;

	// specular color
	vec3 reflectDir = reflect(-frag2Light_norm, normal);
	float specCoe = max(dot(reflectDir, viewDir), 0.0);
	vec3 specular = pow(specCoe, shinness) * objSpecular * light.color;

	return ambient + diffuse + specular;
}

vec3 calPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness)
{
	vec3 frag2Light = light.position - fragPos;
	float distance = length(frag2Light);
	float attenduation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	// diffuse color
	vec3 frag2Light_norm = normalize(frag2Light);
	vec3 diffuse = max(dot(frag2Light_norm, normal), 0.0) * objDiffuse * light.color;

	// ambient color
	vec3 ambient = objAmbient * light.color * vec3(0.9);

	// specular color
	vec3 reflectDir = reflect(-frag2Light_norm, normal);
	float specCoe = max(dot(reflectDir, viewDir), 0.0);
	vec3 specular = pow(specCoe, shinness) * objSpecular * light.color;

	return (ambient + diffuse + specular) * attenduation;
}

vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 objDiffuse, vec3 objAmbient, vec3 objSpecular, float shinness)
{
	vec3 frag2Light = light.position - fragPos;

	// diffuse color
	vec3 frag2Light_norm = normalize(frag2Light);
	vec3 diffuse = max(dot(frag2Light_norm, normal), 0.0) * objDiffuse * light.color;

	float theta = dot(frag2Light_norm, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.3, 1.0);

	// ambient color
	vec3 ambient = objAmbient * light.color * vec3(0.9);

	// specular color
	// viewDir就是frag2Light_norm
	vec3 reflectDir = reflect(-frag2Light_norm, normal);
	float specCoe = max(dot(reflectDir, frag2Light_norm), 0.0);
	vec3 specular = pow(specCoe, shinness) * objSpecular * light.color;

	return (ambient + diffuse + specular) * intensity;
}