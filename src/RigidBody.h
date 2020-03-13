#pragma once
#include "Utils.h"
#include "Shape.h"
#include "Global.h"
#include "Program.h"

/*
 * A shape with collisions and transformations
 */
class RigidBody : public Selectable
{
public:
	RigidBody(Shape* shape);
	RigidBody(Dimension dimensions);
	RigidBody(Shape* shape, Dimension dimensions);

	virtual Shape* getShape();
	virtual Dimension getDimensions();
	virtual Point getPosition();
	virtual Vector getScale();
	virtual Vector getAngles();
	virtual RigidBody* setShape(Shape* shape);
	virtual RigidBody* setDimensions(Dimension dimensions);
	virtual RigidBody* setPosition(Point position);
	virtual RigidBody* setScale(Vector scale);
	virtual RigidBody* setAngles(Vector angles);
	virtual RigidBody* move(Vector delta);
	virtual RigidBody* scale(Vector delta);
	virtual RigidBody* rotate(Vector delta);
	
	virtual bool isColliding(RigidBody *r);
	virtual void onCollision(RigidBody *r);
	virtual void draw();
	virtual void drawExtra();

private:
	Shape* shape;
	Dimension dimensions;
	Point position;
	Vector scaling;
	Vector angles;
};

RigidBody::RigidBody(Shape* shape) : RigidBody(shape, { 0, 0, 0 }) { }

RigidBody::RigidBody(Dimension dimensions) : RigidBody(NULL, dimensions) { }

RigidBody::RigidBody(Shape * shape, Dimension dimensions) : Selectable()
{
	this->shape = shape;
	this->dimensions = dimensions;
	this->position = { 0, 0, 0 };
	this->scaling = { 1, 1, 1 };
	this->angles = { 0, 0, 0 };
}

inline Shape* RigidBody::getShape()
{
	return this->shape;
}

inline Dimension RigidBody::getDimensions()
{
	Dimension effectiveDimensions = dimensions;
	effectiveDimensions.width *= scaling.x;
	effectiveDimensions.height *= scaling.y;
	effectiveDimensions.depth *= scaling.z;
	return effectiveDimensions;
}

inline Point RigidBody::getPosition()
{
	return this->position;
}

inline Vector RigidBody::getScale()
{
	return this->scaling;
}

inline Vector RigidBody::getAngles()
{
	return this->angles;
}

inline RigidBody* RigidBody::setShape(Shape* shape)
{
	this->shape = shape;
	return this;
}

inline RigidBody* RigidBody::setDimensions(Dimension dimensions)
{
	this->dimensions = dimensions;
	return this;
}

inline RigidBody* RigidBody::setPosition(Point position)
{
	this->position = position;
	return this;
}

inline RigidBody* RigidBody::setScale(Vector scale)
{
	this->scaling = scale;
	return this;
}

inline RigidBody* RigidBody::setAngles(Vector angles)
{
	this->angles = angles;
	return this;
}

inline RigidBody* RigidBody::move(Vector delta)
{
	position.x += delta.x;
	position.y += delta.y;
	position.z += delta.z;
	return this;
}

inline RigidBody* RigidBody::scale(Vector delta)
{
	scaling.x += delta.x;
	scaling.y += delta.y;
	scaling.z += delta.z;
	return this;
}

inline RigidBody* RigidBody::rotate(Vector delta)
{
	angles.x += delta.x;
	angles.y += delta.y;
	angles.z += delta.z;
	return this;
}

inline bool RigidBody::isColliding(RigidBody* r)
{
	return (absv(r->getPosition().x - this->getPosition().x) <= (r->getDimensions().width + this->getDimensions().width) / 2.0) &&
		   (absv(r->getPosition().y - this->getPosition().y) <= (r->getDimensions().height + this->getDimensions().height) / 2.0) &&
		   (absv(r->getPosition().z - this->getPosition().z) <= (r->getDimensions().depth + this->getDimensions().depth) / 2.0);
}

inline void RigidBody::onCollision(RigidBody* r) { }

inline void RigidBody::draw()
{
	if (shape == NULL)
		return;

	Material m = isSelected() ? World::SELECTED_MATERIAL : World::DEFAULT_MATERIAL;
	Program::getShader()->setUniformFloat(Shaders::SHININESS_VARIABLE, m.shininess)
		->setUniformVec3(Shaders::AMBIENT_PRODUCT_VARIABLE, World::DEFAULT_LIGHT.ambient * m.ambient)
		->setUniformVec3(Shaders::DIFFUSE_PRODUCT_VARIABLE, World::DEFAULT_LIGHT.diffuse * m.diffuse)
		->setUniformVec3(Shaders::SPECULAR_PRODUCT_VARIABLE, World::DEFAULT_LIGHT.specular * m.specular);
	
	Model* model = Program::getModel();
	model->pushMatrix();
	model->translate(position.x, position.y, position.z);
	model->scale(scaling.x, scaling.y, scaling.z);
	model->rotate(angles.x, 0.0, 1.0, 0.0);
	model->rotate(angles.y, 0.0, 0.0, 1.0);
	model->rotate(angles.z, 1.0, 0.0, 0.0);
	shape->draw();
	drawExtra();
	model->pullMatrix();
}

inline void RigidBody::drawExtra() { }