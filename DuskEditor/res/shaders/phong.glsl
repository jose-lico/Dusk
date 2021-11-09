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

uniform sampler2D u_Diffuse;
uniform vec3 u_DiffuseColor;
uniform sampler2D u_Specular;

uniform vec3 e_LightDirection; // front vector for directional light
uniform vec3 e_LightColor;
uniform vec3 e_ViewPosition;

void main()
{
    // ambient lighting
	float ambientStrength = 0.1;
    vec3 ambient = e_LightColor * vec3(texture(u_Diffuse, TexCoords)) * ambientStrength;

    vec3 lightDir = normalize(-e_LightDirection);
    vec3 diffuse = e_LightColor * max(dot(normalize(Normal), lightDir), 0.0) * vec3(texture(u_Diffuse, TexCoords)) * u_DiffuseColor;

    float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, Normal);
	vec3 viewDir = normalize(e_ViewPosition - FragPos);
	float spec =  pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = e_LightColor * spec * vec3(texture(u_Specular, TexCoords));

    vec3 total = ambient + diffuse + specular;

    FragColor = vec4(total, 1.0);
}