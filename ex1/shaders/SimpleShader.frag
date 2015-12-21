#version 330

uniform vec4 fillColor;

out vec4 outColor;

uniform vec2 center;

uniform float radius;

uniform vec2 lightSource;

uniform vec2 windowSize;

void main()
{
	vec2 FragPos = gl_FragCoord.xy;
	FragPos.x = FragPos.x / windowSize.x * 2 -1;
	FragPos.y = FragPos.y / windowSize.y * 2 -1;

	vec2 lightCenter = (normalize(lightSource - center) * (radius / 2)) + center;
	float distFromCenter  = distance(lightCenter, FragPos.xy);
	outColor = 0.9 - (distFromCenter * fillColor * 35);
}