#version 330

uniform sampler2D DiffuseMap;

in vec2 fragUV;

out vec4 FragColor;

void main()
{
	vec4 Kd = texture2D(DiffuseMap, fragUV.st);
	if (Kd.xyz != 0) FragColor = Kd;
    else FragColor.xyz = vec3(1.0, 0.0, 0.0);
}

