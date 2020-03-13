#version 420 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec4 vertexColor;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 eyePosition;

uniform float shininess;
uniform vec3 lightPosition;
uniform vec3 ambientProduct;
uniform vec3 diffuseProduct;
uniform vec3 specularProduct;

out vec4 Color;

void main()
{
	vec3 M = (model * vec4(vertexPosition, 1.0)).xyz;							// trasforma le coordinate locali in coordinate nel mondo (oggetto)
	vec3 N = normalize(model * vec4(vertexNormal, 1.0)).xyz;					// trasforma le coordinate locali in coordinate nel mondo (normali) e normalizza
	vec3 V = normalize(eyePosition - M);										// calcola la direzione di vista normalizzata

	vec3 L = normalize(lightPosition - M);										// normalizza la direzione della luce
	vec3 R = -normalize(reflect(L, N));											// calcola la direzione di riflessione
	vec3 ambient = ambientProduct;												// componente ambientale
	vec3 diffuse = diffuseProduct * max(dot(L, N), 0.0);						// componenete diffusiva
	vec3 specular = specularProduct * pow(max(dot(R, V), 0.0), shininess);		// componente speculare

	gl_Position =  projection * view * model * vec4(vertexPosition, 1.0);		// trasforma le coordinate del vertice nelle coordinate di clipping
	Color = vec4(ambient + diffuse + specular, 1.0);							// calcola il colore ottenuto e aggiunge il canale alfa
}