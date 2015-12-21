#version 330

uniform vec3 lightColor1 = vec3(1.0, 0.9, 0.7);
uniform vec3 lightColor2 = vec3(0.6, 0.6, 1.0);
uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform vec3 light_position1 = vec3(3.0, 2.0, 1.0);
uniform vec3 light_position2 = vec3(-3.0, 0.0, 1.0);
uniform vec3 ka = vec3(0.1, 0.1, 0.1);
uniform vec3 kd = vec3(0.3, 0.3, 0.3);
uniform vec3 ks = vec3(0.3, 0.3, 0.3);
uniform float specExp;

in vec3 nTemp;
in vec3 shadingPosition;

out vec4 outColor;

void main()
{
    vec3 lightDirection1 = normalize(shadingPosition - light_position1);
    vec3 lightDirection2 = normalize(shadingPosition - light_position2);
    vec3 view = normalize(vec3(0.0, 0.0, 0.0) - shadingPosition);
    vec3 n = normalize(nTemp);
    vec3 diffColor = ka * ambientColor + lightColor1 * kd * max(dot(n, -lightDirection1), 0.0) + lightColor2 * kd *
    				max(dot(n, -lightDirection2), 0.0);
    vec3 reflect1 = normalize(reflect(lightDirection1, n));
    vec3 reflect2 = normalize(reflect(lightDirection2, n));
    vec3 specularColor = ks * pow(max(dot(view, reflect1), 0.0), specExp) * lightColor1 + ks *
    					pow(max(dot(view, reflect2), 0.0), specExp) * lightColor2;
    outColor = vec4(diffColor + specularColor, 1.0);
}