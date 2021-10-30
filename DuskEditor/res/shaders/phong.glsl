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

uniform sampler2D u_Texture;
uniform vec3 u_TestUniform;
uniform vec3 e_LightDirection; // front vector for directional light
uniform vec3 e_LightColor;

void main()
{
    // ambient lighting
	float ambientStrength = 0.2;
    vec3 ambient = u_TestUniform * vec3(texture(u_Texture, TexCoords)) * ambientStrength;

    vec3 lightDir = normalize(e_LightDirection);
    vec3 diffuse = u_TestUniform * max(dot(normalize(Normal), -lightDir), 0.0) * vec3(texture(u_Texture, TexCoords));

    vec3 total = ambient + diffuse;

    FragColor = vec4(total, 1.0);
}