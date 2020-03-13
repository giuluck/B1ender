#pragma once
#include "Utils.h"

/*
 * Container Class for Global Variables
 */

const struct Animation {
	static const int FPS;
	static const int WHEEL_SENSIBILITY;
	static const int MOUSE_SENSIBILITY;
};

const struct World {
	static const int SEMI_WIDTH;
	static const int SEMI_HEIGHT;
	static const double LINE_THICKNESS;
	static const Color BACKGROUND_COLOR;
	static const Light DEFAULT_LIGHT;
	static const Material DEFAULT_MATERIAL;
	static const Material SELECTED_MATERIAL;
};

const struct Window {
	static const string TITLE;
	static const int POSITION_X;
	static const int POSITION_Y;
};

const struct Shaders {
	static const string VERTEX_FILENAME;
	static const string FRAGMENT_FILENAME;
	static const string TIME_VARIABLE;
	static const string MODEL_VARIABLE;
	static const string VIEW_VARIABLE;
	static const string PROJECTION_VARIABLE;
	static const string EYE_POSITION_VARIABLE;
	static const string SHININESS_VARIABLE;
	static const string LIGHT_POSITION_VARIABLE;
	static const string AMBIENT_PRODUCT_VARIABLE;
	static const string DIFFUSE_PRODUCT_VARIABLE;
	static const string SPECULAR_PRODUCT_VARIABLE;
	static const function<void()> DEFAULT_UNIFORM_VARIABLES_ROUTINE;
};

const int Animation::FPS = 60;
const int Animation::WHEEL_SENSIBILITY = 10;
const int Animation::MOUSE_SENSIBILITY = 100;
const int World::SEMI_WIDTH = 480;
const int World::SEMI_HEIGHT = 270;
const double World::LINE_THICKNESS = 2.0;
const Color World::BACKGROUND_COLOR = { 0.0, 0.0, 0.0, 1.0 };
const Light World::DEFAULT_LIGHT = 
{
	{ 0.0, 0.0, -1.0 },
	{ 0.1, 0.1, 0.1 },
	{ 1.0, 1.0, 1.0 },
	{ 0.2, 0.2, 0.2 }
};
const Material World::DEFAULT_MATERIAL = 
{
	1000.0,
	{ 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 }
};
const Material World::SELECTED_MATERIAL =
{
	1000.0,
	{ 5.0, 0.0, 0.0 },
	{ 0.8, 0.5, 0.5 },
	{ 0.8, 0.5, 0.5 }
};
const string Window::TITLE = "B1ender";
const int Window::POSITION_X = 0;
const int Window::POSITION_Y = 0;
const string Shaders::VERTEX_FILENAME = "VectorShaderWithLights.glsl";
const string Shaders::FRAGMENT_FILENAME = "FragmentShader.glsl";
const string Shaders::TIME_VARIABLE = "time";
const string Shaders::MODEL_VARIABLE = "model";
const string Shaders::VIEW_VARIABLE = "view";
const string Shaders::PROJECTION_VARIABLE = "projection";
const string Shaders::EYE_POSITION_VARIABLE = "eyePosition";
const string Shaders::SHININESS_VARIABLE = "shininess";
const string Shaders::LIGHT_POSITION_VARIABLE = "lightPosition";
const string Shaders::AMBIENT_PRODUCT_VARIABLE = "ambientProduct";
const string Shaders::DIFFUSE_PRODUCT_VARIABLE = "diffuseProduct";
const string Shaders::SPECULAR_PRODUCT_VARIABLE = "specularProduct";
const function<void()> Shaders::DEFAULT_UNIFORM_VARIABLES_ROUTINE = []() 
{
	Program::getShader()->setUniformMat4(Shaders::MODEL_VARIABLE, Program::getModel()->getMatrix());
};