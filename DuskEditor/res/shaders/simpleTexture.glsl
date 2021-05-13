#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0);
    TexCoords = textureCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, TexCoords);
}