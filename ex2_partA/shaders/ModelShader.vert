#version 330

layout(location = 0) in vec4 positionModel;
out vec3 axis;
uniform mat4 modelTransform;

void main()
{
    gl_Position = modelTransform * positionModel;
    axis = positionModel.xyz;
}