#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 e_Model;
uniform mat4 e_ViewProjection;

void main()
{
    gl_Position = e_ViewProjection * e_Model * vec4(position, 1.0);
    FragPos = vec3(e_Model * vec4(position, 1.0));
    TexCoords = textureCoords;
    Normal = mat3(transpose(inverse(e_Model))) * normal;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

const int MAX_LIGHTS = 8;

struct DirectionalLight
{
    bool Enabled;

    vec3 Direction;
    vec3 Color;
};

struct PointLight {

    bool Enabled;

    vec3 Color;
    vec3 Position;
    
    float Constant;
    float Linear;
    float Quadratic;
};

uniform sampler2D u_Diffuse;
uniform vec3 u_DiffuseColor;
uniform sampler2D u_Specular;

uniform vec3 e_ViewPosition;
uniform int e_DirectionalLightsCount;
uniform int e_PointLightsCount;
uniform DirectionalLight e_DirectionalLights[MAX_LIGHTS];
uniform PointLight e_PointLights[MAX_LIGHTS];

vec3 CalcDirLight(DirectionalLight light, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 viewDir);

void main()
{
    vec3 total = vec3(0.0);
    vec3 viewDir = normalize(e_ViewPosition - FragPos);

    for(int i = 0; i < e_DirectionalLightsCount; i++)
        total += CalcDirLight(e_DirectionalLights[i], viewDir);

    for(int i = 0; i < e_PointLightsCount; i++)
        total += CalcPointLight(e_PointLights[i], viewDir);

    FragColor = vec4(total, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 viewDir)
{
    if(!light.Enabled)
    {
        return vec3(0.0, 0.0, 0.0);
    }
    vec3 lightDir = normalize(-light.Direction);

    // Ambient
	float ambientStrength = 0.2;
    vec3 ambient = light.Color * ambientStrength;

    // Diffuse
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse = light.Color * diff;

    // Specular
    float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, normalize(Normal));
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.Color * spec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 viewDir)
{
    if(!light.Enabled)
    {
        return vec3(0.0, 0.0, 0.0);
    }
    light.Constant = 1.0;
    light.Linear = 0.09;
    light.Quadratic = 0.032;

    vec3 lightDir = normalize(light.Position - FragPos);
    
    // Diffuse
    float diff = max(dot(normalize(Normal), lightDir), 0.0);
    vec3 diffuse  = light.Color * diff;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normalize(Normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.Color * spec;

    // Attenuation
    float distance = length(light.Position - FragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + 
  			     light.Quadratic * (distance * distance));

    // Combine results
    diffuse  *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
}