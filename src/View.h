#pragma once
#include "Utils.h"

/*
 * Defines the WCS -> VCS Transformation
 */
class View
{
public:
	static const vec3 DEFAULT_POSITION;
	static const vec3 DEFAULT_DIRECTION;
	static const vec3 DEFAULT_NORMAL;

	View();
	View(vec3 position, vec3 direction, vec3 normal);

	vec3 getPosition();
	vec3 getDirection();
	vec3 getNormal();
	vec3 getCenter();
	View* setPosition(double x, double y, double z);
	View* setDirection(double x, double y, double z);
	View* setNormal(double x, double y, double z);
	View* setCenter(double x, double y, double z);

	mat4 getMatrix();
	View* move(double dx, double dy, double dz, bool fixedCenter);

private:
	vec3 position, direction, normal;
};

const vec3 View::DEFAULT_POSITION(0.0, 0.5, -5.0);
const vec3 View::DEFAULT_DIRECTION(0.0, 0.0, -1.0);
const vec3 View::DEFAULT_NORMAL(0.0, 1.0, 0.0);

View::View() : View(DEFAULT_POSITION, DEFAULT_DIRECTION, DEFAULT_NORMAL) { }

View::View(vec3 position, vec3 direction, vec3 normal)
{
	this->position = position;
	this->direction = direction;
	this->normal = normal;
}

inline vec3 View::getPosition()
{
	return position;
}

inline vec3 View::getDirection()
{
	return direction;
}

inline vec3 View::getNormal()
{
	return normal;
}

inline vec3 View::getCenter()
{
	return position - direction;
}

inline View* View::setPosition(double x, double y, double z)
{
	position = { x, y, z };
	return this;
}

inline View* View::setDirection(double x, double y, double z)
{
	direction = { x, y, z };
	return this;
}

inline View* View::setNormal(double x, double y, double z)
{
	normal = { x, y, z };
	return this;
}

inline View* View::setCenter(double x, double y, double z)
{
	direction = DEFAULT_DIRECTION;
	return setPosition(x + DEFAULT_POSITION.x, y + DEFAULT_POSITION.y, z + DEFAULT_POSITION.z);
}

inline mat4 View::getMatrix()
{
	return lookAt(position, getCenter(), normal);
}

inline View* View::move(double dx = 0.0, double dy = 0.0, double dz = 0.0, bool fixedCenter = false)
{
	vec3 d = { dx, dy, dz };
	position += d;

	if (fixedCenter) 
	{
		direction += d;
	}

	return this;
}
