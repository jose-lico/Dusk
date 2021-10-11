#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 TexCoords;
out vec3 Color;

uniform mat4 u_ModelViewProjection;
uniform vec3 u_Color;

void main()
{
    gl_Position = u_ModelViewProjection * vec4(position, 1.0);
    TexCoords = textureCoords;
    Color = u_Color;
}

#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 Color;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, TexCoords) * vec4(Color, 1.0);
}