#version 330

layout(location = 0) in vec4 positionModel;
layout(location = 1) in vec3 normalModel;
out vec3 axis;
uniform mat4 modelTransform;

void main()
{
    gl_Position = modelTransform * positionModel;
    axis = positionModel.xyz;
}