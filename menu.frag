#version 330

#define PI 3.14159265359

in vec2 fragUV;
in vec4 positionVec;
in vec4 normalVec;

out vec4 FragColor;

uniform sampler2D DiffuseMap;
uniform vec3 EyePos;
uniform int NumOfLights;
uniform mat3 LightInfo[16];
uniform float Alpha;
uniform float TextureRepeat;

void main()
{
	vec4 Kd = texture2D(DiffuseMap, fragUV.st);
	FragColor = Kd;
}