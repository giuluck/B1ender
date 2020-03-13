#version 420 core

in vec4 Color;

out vec4 interpolatedColor;

void main()
{
	interpolatedColor = Color;
}