#pragma once
#include "Utils.h"
#include "Model.h"
#include "View.h"
#include "Projection.h"
#include "Shader.h"
#include <stack>

class Program
{
public:
	static void initDefault();
	static Model* getModel();
	static View* getView();
	static Projection* getProjection();
	static Shader* getShader();

	static void pushModel(Model* model);
	static void pushView(View* view);
	static void pushProjection(Projection* projection);
	static void pushShader(Shader* shader);
	static void popModel();
	static void popView();
	static void popProjection();
	static void popShader();

private:
	Program();

	static stack<Model*> models;
	static stack<View*> views;
	static stack<Projection*> projections;
	static stack<Shader*> shaders;
	static Optional<int> selectedBody;
};

stack<Model*> Program::models;
stack<View*> Program::views;
stack<Projection*> Program::projections;
stack<Shader*> Program::shaders;

inline void Program::initDefault()
{
	pushModel(new Model());
	pushView(new View());
	pushProjection(new Projection());
	pushShader(new Shader());
}

inline Model* Program::getModel()
{
	return models.top();
}

inline View* Program::getView()
{
	return views.top();
}

inline Projection* Program::getProjection()
{
	return projections.top();
}

inline Shader * Program::getShader()
{
	return shaders.top();
}

inline void Program::pushModel(Model* model)
{
	models.push(new Model(*model));
}

inline void Program::pushView(View* view)
{
	views.push(new View(*view));
}

inline void Program::pushProjection(Projection* projection)
{
	projections.push(new Projection(*projection));
}

inline void Program::pushShader(Shader* shader)
{
	shaders.push(new Shader(*shader));
}

inline void Program::popModel()
{
	delete getModel();
	models.pop();
	if (models.empty())
		pushModel(new Model());
}

inline void Program::popView()
{
	delete getView();
	views.pop();
	if (views.empty())
		pushView(new View());
}

inline void Program::popProjection()
{
	delete getProjection();
	projections.pop();
	if (projections.empty())
		pushProjection(new Projection());
}

inline void Program::popShader()
{
	delete getShader();
	shaders.pop();
	if (shaders.empty())
		pushShader(new Shader());
}