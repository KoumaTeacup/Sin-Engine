#version 330

//layout (location = 0) in vec3 Position;
in vec3 Position;

uniform mat4 PVM;

void main()
{
    gl_Position = PVM * vec4(Position , 1.0);
}

