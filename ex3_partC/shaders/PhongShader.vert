#version 330

layout(location = 0) in vec4 positionModel;
layout(location = 1) in vec3 normalModel;

uniform mat4 modelTransform;

out vec3 shadingPosition;
out vec3 nTemp;
out vec3 myPosition;

void main()
{
	gl_Position = modelTransform * positionModel;
	myPosition = positionModel.xyz;
	nTemp = (modelTransform * vec4(normalModel, 0.0)).xyz;
	shadingPosition = (modelTransform * positionModel).xyz;
}