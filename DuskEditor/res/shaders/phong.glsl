#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(position, 1.0);
    FragPos = vec3(u_Model * vec4(position, 1.0));
    TexCoords = textureCoords;
    Normal = mat3(transpose(inverse(u_Model))) * normal;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor * vec3(texture(Texture, TexCoords));

    vec3 lightPos = vec3(1.0, -1.0, -1.0);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = lightColor * max(dot(normalize(Normal), -lightDir), 0.0) * vec3(texture(Texture, TexCoords));

    FragColor = vec4(ambient + diffuse, 1.0);
}