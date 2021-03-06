#version 330

layout(location = 0) in vec4 position;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position);
}