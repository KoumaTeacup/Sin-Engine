#version 330

in vec3 vertex;
in vec3 normal;
in vec3 uv;
in vec3 tangent;

uniform mat4 ModelTr;
uniform mat4 ViewTr;
uniform mat4 ProjTr;

void main()
{
    gl_Position = ModelTr * vec4(vertex , 1.0);
    //gl_Position = vec4(vertex , 1.0);
}

