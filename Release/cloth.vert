#version 330

in int id;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;

out vec2 fragUV;

uniform mat4 ModelTr;
uniform mat4 ViewTr;
uniform mat4 ProjTr;

uniform mat4 ClothTr[200];

void main()
{
	fragUV = uv;
    gl_Position =  ProjTr * ViewTr * ModelTr * ClothTr[id] * vec4(vertex , 1.0);
}

