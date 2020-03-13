#pragma once
#include "Utils.h"
#include "Shape.h"

enum DefaultShapes { PLANE = 0, CUBE = 1, PYRAMID = 2, SPHERE = 3, CILINDER = 4, CONE = 5, TORUS = 6 };

/*
 * Utility Class with Main Shapes
 */
class Shapes
{
public:
	// NORMALIZED SHAPES
	static Shape* PLANE;
	static Shape* CUBE;
	static Shape* PYRAMID;
	static Shape* SPHERE;
	static Shape* CILINDER;
	static Shape* CONE;
	static Shape* TORUS;

	// SHAPES FACTORY
	static void initDefault();
	static Shape* plane(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* cube(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* pyramid(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* sphere(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* cilinder(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* cone(Dimension dimensions = { 1.0, 1.0, 1.0 }, Optional<Color> c = Optional<Color>());
	static Shape* torus(Dimension dimensions = { 0.75, 0.75, 0.75 }, Dimension thickness = { 0.25, 0.25, 0.25 }, Optional<Color> c = Optional<Color>());

private:
	static vector<Point> getNormalsVector(vector<Point> v);
	static vector<Color> getColorsVector(Optional<Color> c, unsigned int size);

	Shapes();
};

Shape* Shapes::PLANE;
Shape* Shapes::CUBE;
Shape* Shapes::PYRAMID;
Shape* Shapes::SPHERE;
Shape* Shapes::CILINDER;
Shape* Shapes::CONE;
Shape* Shapes::TORUS;

inline void Shapes::initDefault()
{
	PLANE = Shapes::plane();
	CUBE = Shapes::cube();
	PYRAMID = Shapes::pyramid();
	SPHERE = Shapes::sphere();
	CILINDER = Shapes::cilinder();
	CONE = Shapes::cone();
	TORUS = Shapes::torus();
}

inline Shape* Shapes::plane(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	vertices.push_back({ -x, 0.0,  z });
	vertices.push_back({  x, 0.0,  z });
	vertices.push_back({  x, 0.0, -z });
	vertices.push_back({ -x, 0.0, -z });

	vector<Index> indices;
	indices.push_back({ 0, 1, 2 });
	indices.push_back({ 0, 2, 3 });

	vector<Point> normals;
	normals.push_back({ 0.0, 1.0, 0.0 });
	normals.push_back({ 0.0, 1.0, 0.0 });
	normals.push_back({ 0.0, 1.0, 0.0 });
	normals.push_back({ 0.0, 1.0, 0.0 });

	return new Shape(vertices, normals, getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::cube(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	vertices.push_back({ -x, -y,  z });
	vertices.push_back({  x, -y,  z });
	vertices.push_back({  x,  y,  z });
	vertices.push_back({ -x,  y,  z });
	vertices.push_back({ -x, -y, -z });
	vertices.push_back({  x, -y, -z });
	vertices.push_back({  x,  y, -z });
	vertices.push_back({ -x,  y, -z });

	vector<Index> indices;
	indices.push_back({ 0, 1, 2 });
	indices.push_back({ 2, 3, 0 });
	indices.push_back({ 1, 5, 6 });
	indices.push_back({ 6, 2, 1 });
	indices.push_back({ 7, 6, 5 });
	indices.push_back({ 5, 4, 7 });
	indices.push_back({ 4, 0, 3 });
	indices.push_back({ 3, 7, 4 });
	indices.push_back({ 4, 5, 1 });
	indices.push_back({ 1, 0, 4 });
	indices.push_back({ 3, 2, 6 });
	indices.push_back({ 6, 7, 3 });

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::pyramid(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	vertices.push_back({ -x, -y,  z });
	vertices.push_back({  x, -y,  z });
	vertices.push_back({  x, -y, -z });
	vertices.push_back({ -x, -y, -z });
	vertices.push_back({  0,  y,  0 });

	vector<Index> indices;
	indices.push_back({ 0, 1, 2 });
	indices.push_back({ 0, 2, 3 });
	indices.push_back({ 0, 4, 3 });
	indices.push_back({ 0, 1, 4 });
	indices.push_back({ 3, 2, 4 });
	indices.push_back({ 1, 2, 4 });

	vector<Point> normals;
	normals.push_back({ 0.0, -1.0, 0.0 });
	normals.push_back({ 0.0, -1.0, 0.0 });
	normals.push_back({ 0.0, -1.0, 0.0 });
	normals.push_back({ 0.0, -1.0, 0.0 });
	normals.push_back({ 0.0,  1.0, 0.0 });

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::sphere(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	unsigned int stacks = 30, slices = 30;
	double phi, theta;
	for (unsigned int i = 0; i <= stacks; i++) {
		phi = i * glm::pi<double>() / stacks;
		for (unsigned int j = 0; j <= slices; j++) {
			theta = 2 * j * glm::pi<double>() / slices;
			vertices.push_back({ x * cos(theta) * sin(phi), y * cos(phi), z * sin(theta) * sin(phi) });
		}
	}

	vector<Index> indices;
	for (unsigned int i = 0; i < slices * (stacks + 1); i++) {
		indices.push_back({ i, i + slices + 1, i + slices });
		indices.push_back({ i + slices + 1, i, i + 1 });;
	}

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::cilinder(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	unsigned int stacks = 30, slices = 30;
	double h, theta;
	for (unsigned int i = 0; i <= stacks; i++) {
		h = i / double(stacks);
		for (unsigned int j = 0; j <= slices; j++) {
			theta = 2 * j * glm::pi<double>() / slices;
			vertices.push_back({ x * cos(theta), y * (h - 0.5), z * sin(theta) });
		}
	}

	vector<Index> indices;
	for (unsigned int i = 0; i < slices * (stacks + 1); i++) {
		indices.push_back({ i, i + slices + 1, i + slices });
		indices.push_back({ i + slices + 1, i, i + 1 });
	}

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::cone(Dimension dimensions, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;

	vector<Point> vertices;
	unsigned int stacks = 30, slices = 30;
	double h, theta;
	for (unsigned int i = 0; i <= stacks; i++) {
		h = i / double(stacks);
		for (unsigned int j = 0; j <= slices; j++) {
			theta = 2 * j * glm::pi<double>() / slices;
			vertices.push_back({ x * h * cos(theta), y * (h - 0.5), z * h * sin(theta) });
		}
	}

	vector<Index> indices;
	for (unsigned int i = 0; i < slices * (stacks + 1); i++) {
		indices.push_back({ i, i + slices + 1, i + slices });
		indices.push_back({ i + slices + 1, i, i + 1 });
	}

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline Shape* Shapes::torus(Dimension dimensions, Dimension thickness, Optional<Color> c)
{
	double x = dimensions.width / 2.0;
	double y = dimensions.height / 2.0;
	double z = dimensions.depth / 2.0;
	double tx = thickness.width / 2.0;
	double ty = thickness.height / 2.0;
	double tz = thickness.depth / 2.0;

	vector<Point> vertices;
	unsigned int stacks = 30, slices = 30;
	double phi, theta;
	for (unsigned int i = 0; i <= stacks; i++) {
		phi = 2 * i * glm::pi<double>() / stacks;
		for (unsigned int j = 0; j <= slices; j++) {
			theta = 2 * j * glm::pi<double>() / slices;
			vertices.push_back({ (x + tx * cos(phi)) * cos(theta),  (y + ty * sin(phi)), (z + tz * cos(phi))* sin(theta) });
		}
	}

	vector<Index> indices;
	for (unsigned int i = 0; i < slices * (stacks + 1); i++) {
		indices.push_back({ i, i + slices + 1, i + slices });
		indices.push_back({ i + slices + 1, i, i + 1 });
	}

	return new Shape(vertices, getNormalsVector(vertices), getColorsVector(c, vertices.size()), indices);
}

inline vector<Point> Shapes::getNormalsVector(vector<Point> v) {
	vector<Point> normals;
	for (unsigned int i = 0; i < v.size(); i++) {
		normals.push_back(v.at(i));
	}
	return normals;
}

inline vector<Color> Shapes::getColorsVector(Optional<Color> c, unsigned int size) {
	vector<Color> colors;
	for (unsigned int i = 0; i < size; i++)
		colors.push_back(c.orElse({ rand(0.0, 1.0), rand(0.0, 1.0), rand(0.0, 1.0), 1.0 }));
	return colors;
}

