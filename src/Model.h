#pragma once
#include "Utils.h"
#include <stack>

/*
 * Defines the OCS -> WCS Transformation
 */
class Model
{
public:
	static const mat4 IDENTITY;

	Model();
	Model* translate(double x, double y, double z);
	Model* scale(double x, double y, double z);
	Model* rotate(double angle, double x, double y, double z);
	Model* pushMatrix();
	Model* pullMatrix();
	mat4 getMatrix();

private:
	stack<mat4> matrices;

	Model* changeMatrix(mat4 newMatrix);
};

const mat4 Model::IDENTITY(1.0);

Model::Model()
{
	matrices.push(IDENTITY);
}

inline Model* Model::translate(double x, double y, double z)
{
	return changeMatrix(glm::translate(getMatrix(), vec3(x, y, z)));
}

inline Model* Model::scale(double x, double y, double z)
{
	return changeMatrix(glm::scale(getMatrix(), vec3(x, y, z)));;
}

inline Model* Model::rotate(double angle, double x, double y, double z)
{
	return changeMatrix(glm::rotate(getMatrix(), float(radians(angle)), vec3(x, y, z)));;
}

inline Model* Model::pushMatrix()
{
	matrices.push(getMatrix());
	return this;
}

inline Model* Model::pullMatrix()
{
	matrices.pop();
	if (matrices.empty())
		matrices.push(IDENTITY);
	return this;
}

inline mat4 Model::getMatrix()
{
	return matrices.top();
}

inline Model* Model::changeMatrix(mat4 newMatrix)
{
	matrices.pop();
	matrices.push(newMatrix);
	return this;
}
