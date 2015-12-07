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
	const vec3 I = vec3(1.0, 1.0, 1.0);

	vec3 Kd = vec3(1.0, 1.0, 1.0);
	vec3 Ks = vec3(1.0, 1.0, 1.0);

	// Diffuse Texture
	if(textureSize(DiffuseMap,0).y > 1) {
		Kd = texture2D(DiffuseMap, fragUV.st*TextureRepeat).rgb;
	}

	vec3 totalLight = vec3(0.0, 0.0, 0.0);

	// Lighting
	vec3 L;
	vec3 H;
	vec3 V = normalize(EyePos - positionVec.xyz);
	vec3 N = normalize(normalVec.xyz);

	vec3 F;
	float D;
	float G;
	vec3 BRDF;
	
	float dN_L;
	float dH_L;
	float decay;
	vec3 decayVec;
	for (int i=0; i<NumOfLights; ++i){
		L = normalize(LightInfo[i][0] - positionVec.xyz);
		H = normalize(L+V);

		dN_L = max(0.0, dot(N, L));
		dH_L = max(0.0, dot(L, H));

		F = Ks + (I - Ks) * pow(1 - dH_L, 5);
		D = (Alpha + 2) / 2 / PI * pow(max(0.0, dot(N, H)), Alpha);
		G = 1 / dH_L / dH_L;
		BRDF = Kd/PI + (F * D * G / 4);
		
		if(LightInfo[i][2].x > 0.0){
			decay = 1 / distance(LightInfo[i][0], positionVec.xyz);
			decay = pow(decay, dot(LightInfo[i][2],I));
		} else decay = 1.0;
		totalLight += dN_L * LightInfo[i][1] * BRDF * decay + Kd * 0.01;
	}

	FragColor.xyz = totalLight;
}

