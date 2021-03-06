// Project3D.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "pch.h"
#include "Program.h"
#include "Global.h"
#include "Shape.h"
#include "Shapes.h"
#include "RigidBody.h"
#include "EditManager.h"

#define TAB 9
#define ESC 27
#define SPACEBAR 32
#define CANC 127

Model* model;
View* view;
Projection* projection;
Shader* shader;
EditManager* manager;

vector<RigidBody*> bodies;

void init(void)
{
	Program::initDefault();
	Shapes::initDefault();

	model = Program::getModel();
	view = Program::getView();
	projection = Program::getProjection();
	shader = Program::getShader();
	
	shader->addShader(GL_VERTEX_SHADER, Shaders::VERTEX_FILENAME)
		->addShader(GL_FRAGMENT_SHADER, Shaders::FRAGMENT_FILENAME)->updateProgram()
		->setVariableLocation(Shaders::TIME_VARIABLE)
		->setVariableLocation(Shaders::MODEL_VARIABLE)
		->setVariableLocation(Shaders::VIEW_VARIABLE)
		->setVariableLocation(Shaders::PROJECTION_VARIABLE)
		->setVariableLocation(Shaders::SHININESS_VARIABLE)
		->setVariableLocation(Shaders::EYE_POSITION_VARIABLE)
		->setVariableLocation(Shaders::LIGHT_POSITION_VARIABLE)
		->setVariableLocation(Shaders::AMBIENT_PRODUCT_VARIABLE)
		->setVariableLocation(Shaders::DIFFUSE_PRODUCT_VARIABLE)
		->setVariableLocation(Shaders::SPECULAR_PRODUCT_VARIABLE);

	manager = new EditManager(&bodies);
	bodies.push_back(new RigidBody(Shapes::plane()));
	bodies.push_back(new RigidBody(Shapes::cube()));
	bodies.at(0)->setScale({ 10.0, 1.0, 10.0 })->setPosition({ 0.0, -0.5, 0.0 });
}

void free(void)
{
	delete model;
	delete view;
	delete projection;
	delete shader;
	delete manager;
	for (RigidBody* body : bodies) 
	{
		delete body;
	}
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (RigidBody* body : bodies)
	{
		body->draw();
	}

	glutSwapBuffers();
}

void update(int i)
{
	Program::getShader()->setUniformFloat(Shaders::TIME_VARIABLE, GLfloat(glutGet(GLUT_ELAPSED_TIME)))
		->setUniformMat4(Shaders::VIEW_VARIABLE, Program::getView()->getMatrix())
		->setUniformMat4(Shaders::PROJECTION_VARIABLE, Program::getProjection()->getMatrix())
		->setUniformVec3(Shaders::EYE_POSITION_VARIABLE, Program::getView()->getPosition())
		->setUniformVec3(Shaders::LIGHT_POSITION_VARIABLE, Program::getView()->getPosition()); // World::DEFAULT_LIGHT.position
	drawScene();
	glutTimerFunc(1000 / Animation::FPS, update, 0);
}

void resize(int w, int h) 
{
	glViewport(0, 0, w, h);
	projection->setAspectRatio(1.0 * w / h);
}

/*
 * WHEEL MOVEMENT -> AXIS MOVEMENT
 */
void mouseWheel(int wheel, int direction, int x, int y)
{
	if (manager->isAxe(NOAXE) || manager->isAxe(ALL))
		return;

	if (manager->isNotEdit(CAMERA_MOVING))
		manager->setEdit(CAMERA_MOVING);
	
	Vector m = manager->getValueOnAxe(double(direction) / Animation::WHEEL_SENSIBILITY);
	view->move(m.x, m.y, m.z);
}

/*
 * WHEEL CLICK (when Mouse Movement) -> DIRECTION MOVEMENT
 */
void mousePressed(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_MIDDLE_BUTTON:
		manager->setEdit(state == GLUT_DOWN ? CAMERA_FIXING : NOEDIT, { x, y });
		break;
	case GLUT_RIGHT_BUTTON:
		manager->backup();
		break;
	default:
		break;
	}
}

/*
 * MOUSE MOVEMENT (if Wheel Click) -> DIRECTION MOVEMENT
 */
void mouseMotion(int x, int y)
{
	if (manager->isEdit(CAMERA_FIXING))
	{
		vec2 p = manager->getMousePosition()->get();
		view->move((x - p.x) / Animation::MOUSE_SENSIBILITY, (y - p.y) / Animation::MOUSE_SENSIBILITY, 0.0, true);
		manager->getMousePosition()->set({ x, y });
	}
}

double normalizeMouse(double m)
{
	return m * (manager->isEdit(ROTATION) ? 180.0 : 1.0) / Animation::MOUSE_SENSIBILITY;
}

/*
 * MOUSE MOVEMENT(if Edit Mode) -> BODY EDITING
 */
void mousePassive(int x, int y)
{
	if (!manager->onObjectEditing())
		return;

	double nx = normalizeMouse(manager->getMousePosition()->get().x - x);
	double ny = normalizeMouse(manager->getMousePosition()->get().y - y);
	Vector values;
	if (manager->isNotAxe(ALL))
		values = manager->getValueOnAxe(maxAbs(nx, ny));
	else if (manager->isNotEdit(SCALING))
		values = { nx, ny , minAbs(nx, ny) };
	else
		values = EditManager::getValueOnAxe(maxAbs(nx, ny), ALL);

	switch (manager->getEdit())
	{
	case TRASLATION:
		manager->getSelected()->getElement()->move(values);
		break;
	case SCALING:
		manager->getSelected()->getElement()->scale(values);
		break;
	case ROTATION:
		manager->getSelected()->getElement()->rotate(values);
		break;
	default:
		break;
	}

	manager->getMousePosition()->set({ x, y });
}

void specialKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_RIGHT:
		manager->setEdit(NOEDIT);
		manager->getSelected()->selectNext();
		break;
	case GLUT_KEY_DOWN:
	case GLUT_KEY_LEFT:
		manager->setEdit(NOEDIT);
		manager->getSelected()->selectPrevious();
		break;
	default:
		break;
	}

	manager->setCheckpoint();
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) 
	{
	case 'n':
	case 'N':
		Program::popView();
		Program::pushView(new View());
		view = Program::getView();
		break;
	case '.':
		if (manager->getSelected()->isPresent())
		{
			Point p = manager->getSelected()->getElement()->getPosition();
			view->setCenter(p.x, p.y, p.z);
		}
		break;
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
	case 'z':
	case 'Z':
		manager->setAxe(key);
		break;
	case 't':
	case 'T':
	case 's':
	case 'S':
	case 'r':
	case 'R':
		manager->setAxe(manager->isEdit(key) ? NOAXE : ALL);
		manager->setEdit(manager->isEdit(key) ? '-' : key, { x, y });
		break;
	case TAB:
		manager->setEdit(NOEDIT);
		(glutGetModifiers() & GLUT_ACTIVE_SHIFT) ? manager->getSelected()->selectPrevious() : manager->getSelected()->selectNext();
		break;
	case SPACEBAR:
		manager->setEdit(NOEDIT);
		manager->getSelected()->isPresent() ? manager->getSelected()->deselect() : manager->getSelected()->reselect();
		break;
	case CANC:
		if (manager->getSelected()->isPresent())
		{
			int index = manager->getSelected()->getIndex();
			RigidBody* body = manager->getSelected()->getElement();
			if (bodies.size() == 1)
				manager->getSelected()->deselect();
			else if (index != 0)
				manager->getSelected()->selectPrevious();
			bodies.erase(bodies.begin() + index);
			delete body;
			manager->getSelected()->reselect();
		}
		break;
	case ESC:
		free();
		exit(0);
		break;
	default:
		break;
	}

	manager->setCheckpoint();
}

void keyReleased(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'x':
	case 'X':
	case 'y':
	case 'Y':
	case 'z':
	case 'Z':
		manager->setAxe(NOAXE);
		break;
	case 't':
	case 'T':
	case 's':
	case 'S':
	case 'r':
	case 'R':
		manager->isAxe(ALL) ? manager->setAxe(NOAXE) : manager->setEdit(NOEDIT);
		break;
	default:
		break;
	}
}

void menuManager(int entry)
{
	switch (entry)
	{
	case PLANE:
		bodies.push_back(new RigidBody(Shapes::plane()));
		break;
	case CUBE:
		bodies.push_back(new RigidBody(Shapes::cube()));
		break;
	case PYRAMID:
		bodies.push_back(new RigidBody(Shapes::pyramid()));
		break;
	case SPHERE:
		bodies.push_back(new RigidBody(Shapes::sphere()));
		break;
	case CILINDER:
		bodies.push_back(new RigidBody(Shapes::cilinder()));
		break;
	case CONE:
		bodies.push_back(new RigidBody(Shapes::cone()));
		break;
	case TORUS:
		bodies.push_back(new RigidBody(Shapes::torus()));
		break;
	default:
		throw "Unknow Shape";
	}

	manager->getSelected()->set(bodies.size() - 1);
	manager->setCheckpoint();
}

int main(int argc, char **argv)
{
	srand(unsigned int(time(0)));

	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(2 * World::SEMI_WIDTH, 2 * World::SEMI_HEIGHT);
	glutInitWindowPosition(Window::POSITION_X, Window::POSITION_Y);
	glutCreateWindow(Window::TITLE.c_str());
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutTimerFunc(1000 / Animation::FPS, update, 0);

	glutIgnoreKeyRepeat(1);
	glutMouseWheelFunc(mouseWheel);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassive);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);

	glEnable(GL_DEPTH_TEST);
	glewExperimental = GL_TRUE;
	glewInit();

	glutCreateMenu(menuManager);
	glutAddMenuEntry("Plane", DefaultShapes::PLANE);
	glutAddMenuEntry("Cube", DefaultShapes::CUBE);
	glutAddMenuEntry("Pyramid", DefaultShapes::PYRAMID);
	glutAddMenuEntry("Sphere", DefaultShapes::SPHERE);
	glutAddMenuEntry("Cilinder", DefaultShapes::CILINDER);
	glutAddMenuEntry("Cone", DefaultShapes::CONE);
	glutAddMenuEntry("Torus", DefaultShapes::TORUS);
	glutAttachMenu(GLUT_LEFT_BUTTON);

	init();
	glutMainLoop();

	return 0;
}