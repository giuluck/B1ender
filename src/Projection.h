#pragma once
#include "Utils.h"

/*
 * Defines the VCS -> NCS Transformation
 */
class Projection
{
public:
	static const double DEFAULT_FOV;
	static const double DEFAULT_NEAR;
	static const double DEFAULT_FAR;

	Projection();
	Projection(double aspectRatio);
	Projection(double aspectRatio, double fieldOfView, double zNear, double zFar);
	double getAspectRatio();
	double getFieldOfView();
	Pair<double, double> getPlanes();
	Projection* setAspectRatio(double aspectRatio);
	Projection* setFieldOfView(double fieldOfView);
	Projection* setPlanes(double zNear, double zFar);
	mat4 getMatrix();

private:
	double aspectRatio, fieldOfView, zNear, zFar;
};

const double Projection::DEFAULT_FOV = radians(45.0);
const double Projection::DEFAULT_NEAR = 0.2;
const double Projection::DEFAULT_FAR = 100.0;

inline Projection::Projection() : Projection(0.0) { }

Projection::Projection(double aspectRatio) : Projection(aspectRatio, DEFAULT_FOV, DEFAULT_NEAR, DEFAULT_FAR) { }

inline Projection::Projection(double aspectRatio, double fieldOfView, double zNear, double zFar)
{
	this->aspectRatio = aspectRatio;
	this->fieldOfView = fieldOfView;
	this->zNear = zNear;
	this->zFar = zFar;
}

inline double Projection::getAspectRatio()
{
	return this->aspectRatio;
}

inline double Projection::getFieldOfView()
{
	return this->fieldOfView;
}

inline Pair<double, double> Projection::getPlanes()
{
	return { this->zNear, this->zFar };
}

inline Projection* Projection::setAspectRatio(double aspectRatio)
{
	this->aspectRatio = aspectRatio;
	return this;
}

inline Projection* Projection::setFieldOfView(double fieldOfView)
{
	this->fieldOfView = fieldOfView;
	return this;
}

inline Projection* Projection::setPlanes(double zNear, double zFar)
{
	this->zNear = zNear;
	this->zFar = zFar;
	return this;
}

inline mat4 Projection::getMatrix()
{
	return perspective(fieldOfView, aspectRatio, zNear, zFar);
}
