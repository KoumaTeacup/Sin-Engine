#version 330

in int	id;
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;

out vec2 fragUV;
out vec4 positionVec;
out vec4 normalVec;

uniform mat4 ModelTr;
uniform mat4 ViewTr;
uniform mat4 ProjTr;

void main()
{
	fragUV = uv;
	positionVec = ModelTr * vec4(vertex, 1.0);
	normalVec = ModelTr * vec4(normal, 0.0);

    gl_Position =  ProjTr * ViewTr * ModelTr * vec4(vertex , 1.0);
}

