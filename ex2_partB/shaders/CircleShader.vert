#version 330

layout(location = 0) in vec4 position;

out vec2 pos;
uniform mat4 circleTransform;

void main()
{
    gl_Position = circleTransform * position;
}