#pragma once

/*
 * Header File for Definition, Inclusions, Structs and Utility Functions
 */

#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

using namespace glm;
using namespace std;

struct Point {
	double x, y, z;
	Point() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	Point(double px, double py, double pz) {
		x = px;
		y = py;
		z = pz;
	}
	bool operator==(Point o) {
		return x == o.x && y == o.y && z == o.z;
	}
};

typedef Point Vector;

struct Dimension {
	double width = 0.0, height = 0.0, depth = 0.0;
	bool operator==(Dimension o) {
		return width == o.width && height == o.height && depth == o.depth;
	}
};

struct Color {
	double r = 1.0, g = 1.0, b = 1.0, a = 1.0;
	bool operator==(Color o) {
		return r == o.r && g == o.g && b == o.b && a == o.a;
	}
};

struct Index {
	GLuint i = 0, j = 0, k = 0;
	bool operator==(Index o) {
		return i == o.i && j == o.j && k == o.k;
	}
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

template <class X = double, class Y = double>
struct Pair
{
	X first;
	Y second;
	bool operator==(Pair<X, Y> o) {
		return first == o.first && second == o.second;
	}
};

template <class X = double, class Y = double, class Z = double>
struct Triple
{
	X first;
	Y second;
	Z third;
	bool operator==(Triple<X, Y, Z> o) {
		return first == o.first && second == o.second && third == o.third;
	}
};

template <class T>
class Optional
{
public:
	Optional() {
		this->isEmpty = true;
	};
	Optional(T value) {
		this->isEmpty = false;
		this->value = value;
	}
	T get() {
		return isPresent() ? value : throw "empty optional cannot be accessed";
	}
	T orElse(T defaultValue) {
		return isPresent() ? value : defaultValue;
	}
	bool isPresent() {
		return !isEmpty;
	}
	bool isNotPresent() {
		return isEmpty;
	}
	void set(T value) {
		this->isEmpty = false;
		this->value = value;
	}
	void empty() {
		this->isEmpty = true;
	}
	void unEmpty() {
		this->isEmpty = false;
	}
	bool operator==(Optional<T> *o) {
		return (o->empty() && this->empty()) || (!o->empty() && !this->empty() && o->get() == this->get());
	}

private:
	bool isEmpty;
	T value;
};

class Selectable
{
public:
	Selectable() : Selectable(false) { }
	Selectable(bool selected)
	{
		this->selected = selected;
	}
	bool isSelected()
	{
		return selected;
	}
	void setSelected(bool selected)
	{
		this->selected = selected;
	}

private:
	bool selected;
};

template <class S>
class Selector
{
public:
	Selector(vector<S> *v) {
		this->v = v;
		this->i = Optional<int>(0);
		i.empty();
	}
	bool isPresent() {
		return i.isPresent();
	}
	bool isNotPresent() {
		return i.isNotPresent();
	}
	int getIndex() {
		if (v->size() == 0)
			return -1;
		return i.get();
	}
	S getElement() {
		if (v->size() == 0)
			return NULL;
		return v->at(getIndex());
	}
	void deselect() {
		if (v->size() == 0)
			return;
		if (isPresent() && v->size() > 0)
			getElement()->setSelected(false);
		i.empty();
	}
	void reselect() {
		if (v->size() == 0)
			return;
		i.unEmpty();
		getElement()->setSelected(true);
	}
	void set(int index) {
		if (v->size() == 0)
			return;
		deselect();
		i.set(index);
		getElement()->setSelected(true);
	}
	void selectNext() {
		if (v->size() == 0)
			return;
		reselect();
		set((getIndex() + 1) % v->size());
	}
	void selectPrevious() {
		if (v->size() == 0)
			return;
		reselect();
		set((getIndex() + v->size() - 1) % v->size());
	}

private:
	vector<S> *v;
	Optional<int> i;
};

double absv(double x)
{
	return x > 0 ? x : -x;
}

double sign(double x)
{
	return x == 0 ? 0 : x / absv(x);
}

double rand(double min, double max)
{
	return min + (max - min) * rand() / RAND_MAX;
}

double minAbs(double a, double b)
{
	return absv(a) < absv(b) ? a : b;
}

double maxAbs(double a, double b)
{
	return absv(a) > absv(b) ? a : b;
}
