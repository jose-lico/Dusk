#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 0) in vec3 color;

out vec3 Color;

void main()
{
    gl_Position = vec4(position, 1.0);
    ourColor = color;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(Color, 1.0f);
}