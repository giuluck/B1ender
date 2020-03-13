#pragma once
#include "Utils.h"
#include "Program.h"

/*
 * Generic 3D Shape
 */
class Shape
{
public:
	Shape();
	Shape(vector<Point> v, vector<Point> n, vector<Index> i);
	Shape(vector<Point> v, vector<Color> c, vector<Index> i);
	Shape(vector<Point> v, vector<Point> n, vector<Color> c, vector<Index> i);
	virtual ~Shape();
	virtual void draw();
	virtual void draw(function<void()> &uniformVariableRoutine);

	virtual void updateVAO();
	virtual vector<Point>* getVertices();
	virtual vector<Point>* getNormals();
	virtual vector<Color>* getColors();
	virtual vector<Index>* getIndices();

private:
	GLuint shapeVAO;
	GLuint verticesVBO, normalsVBO, colorsVBO, indicesVBO;
	vector<Point> vertices;
	vector<Point> normals;
	vector<Color> colors;
	vector<Index> indices;

	void createVAO();
	void deleteVAO();
};

Shape::Shape() { }

Shape::Shape(vector<Point> v, vector<Point> n, vector<Index> i)
{
	vertices.insert(vertices.end(), v.begin(), v.end());
	normals.insert(normals.end(), n.begin(), n.end());
	indices.insert(indices.end(), i.begin(), i.end());

	for (unsigned int i = 0; i < v.size(); i++)
	{
		colors.push_back({ 1.0, 1.0, 1.0, 0.0 });
	}

	createVAO();
}

Shape::Shape(vector<Point> v, vector<Color> c, vector<Index> i)
{
	vertices.insert(vertices.end(), v.begin(), v.end());
	colors.insert(colors.end(), c.begin(), c.end());
	indices.insert(indices.end(), i.begin(), i.end());

	for (unsigned int i = 0; i < v.size(); i++) 
	{
		normals.push_back({ 0.0, 0.0, 0.0 });
	}

	createVAO();
}

inline Shape::Shape(vector<Point> v, vector<Point> n, vector<Color> c, vector<Index> i)
{
	vertices.insert(vertices.end(), v.begin(), v.end());
	normals.insert(normals.end(), n.begin(), n.end());
	colors.insert(colors.end(), c.begin(), c.end());
	indices.insert(indices.end(), i.begin(), i.end());

	createVAO();
}

Shape::~Shape()
{
	deleteVAO();
}

inline void Shape::draw()
{
	function<void()> routine = Shaders::DEFAULT_UNIFORM_VARIABLES_ROUTINE;
	draw(routine);
}

inline void Shape::draw(function<void()>& uniformVariableRoutine)
{
	uniformVariableRoutine();
	glBindVertexArray(shapeVAO);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(Index), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

inline void Shape::updateVAO()
{
	deleteVAO();
	createVAO();
}

inline vector<Point>* Shape::getVertices()
{
	return &vertices;
}

inline vector<Point>* Shape::getNormals()
{
	return &normals;
}

inline vector<Color>* Shape::getColors()
{
	return &colors;
}

inline vector<Index>* Shape::getIndices()
{
	return &indices;
}

inline void Shape::createVAO()
{
	glGenVertexArrays(1, &shapeVAO);
	glBindVertexArray(shapeVAO);

	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Point), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Point), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), &colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_DOUBLE, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

inline void Shape::deleteVAO()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDeleteVertexArrays(1, &shapeVAO);
	glDeleteBuffers(1, &verticesVBO);
	glDeleteBuffers(1, &normalsVBO);
	glDeleteBuffers(1, &colorsVBO);
	glDeleteBuffers(1, &indicesVBO);
}