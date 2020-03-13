#version 420 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 MVP;
uniform float time;

out vec4 Color;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1.0);
	Color = vertexColor;
}