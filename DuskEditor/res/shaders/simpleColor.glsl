#shader vertex
#version 330 core

layout (location = 0) in vec3 position;

out vec3 Color;

uniform vec3 u_Color;
uniform mat4 e_Model;
uniform mat4 e_ViewProjection;

void main()
{
    gl_Position = e_ViewProjection * e_Model * vec4(position, 1.0);
    Color = u_Color;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Color;

void main()
{
    FragColor = vec4(Color, 1.0f);
}