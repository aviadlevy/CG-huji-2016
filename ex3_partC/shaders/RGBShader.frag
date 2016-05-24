#version 330

out vec4 outColorModel;
in vec3 axis;

void main()
{
	outColorModel.xyz = axis * vec3(0.5f	) + vec3(0.5f);
}