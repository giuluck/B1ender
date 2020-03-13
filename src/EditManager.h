#pragma once
#include "Utils.h"
#include "RigidBody.h"

enum Edit { NOEDIT = '-', CAMERA_MOVING = 'M', CAMERA_FIXING = 'F', TRASLATION = 'T', SCALING = 'S', ROTATION = 'R' };
enum Axe { NOAXE = '-', X = 'X', Y = 'Y', Z = 'Z', ALL = 'A' };

class EditManager
{
public:
	static Vector getValueOnAxe(double value, Axe axe);

	EditManager(vector<RigidBody*> *bodies);
	~EditManager();

	Edit getEdit();
	Axe getAxe();
	Optional<vec2>* getMousePosition();
	Selector<RigidBody*>* getSelected();
	EditManager* deleteSelected();
	EditManager* setEdit(Edit edit, vec2 position = { 0.0, 0.0 });
	EditManager* setEdit(char edit, vec2 position = { 0.0, 0.0 });
	EditManager* setAxe(Axe axe);
	EditManager* setAxe(char axe);

	EditManager* setCheckpoint();
	EditManager* backup();

	bool isEdit(Edit edit);
	bool isEdit(char edit);
	bool isAxe(Axe axe);
	bool isAxe(char axe);
	bool isNotEdit(Edit edit);
	bool isNotEdit(char edit);
	bool isNotAxe(Axe axe);
	bool isNotAxe(char axe);

	bool onObjectEditing();
	bool onAxeEnabled();
	Vector getValueOnAxe(double value);

private:
	Edit edit;
	Axe axe;
	Optional<vec2> mousePosition;
	Optional<RigidBody*> backupBody;
	Selector<RigidBody*>* selectedBody;

	void log();
	string editString();
	string axeString();
};

inline Vector EditManager::getValueOnAxe(double value, Axe axe)
{
	switch (axe)
	{
	case X:
		return Vector(value, 0.0, 0.0);
	case Y:
		return Vector(0.0, value, 0.0);
	case Z:
		return Vector(0.0, 0.0, value);
	case ALL:
		return Vector(value, value, value);
	default:
		return Vector(0.0, 0.0, 0.0);
	}
}

inline EditManager::EditManager(vector<RigidBody*>* bodies)
{
	mousePosition = Optional<vec2>();
	backupBody = Optional<RigidBody*>();
	selectedBody = new Selector<RigidBody*>(bodies);
	setAxe(NOAXE);
	setEdit(NOEDIT);
}

inline EditManager::~EditManager()
{
	delete selectedBody;
}

inline Edit EditManager::getEdit()
{
	return edit;
}

inline Axe EditManager::getAxe()
{
	return axe;
}

inline Optional<vec2>* EditManager::getMousePosition()
{
	return &mousePosition;
}

inline Selector<RigidBody*>* EditManager::getSelected()
{
	return selectedBody;
}

inline EditManager* EditManager::deleteSelected()
{
	return this;
}

inline EditManager* EditManager::setEdit(Edit edit, vec2 position)
{
	if ((edit == TRASLATION || edit == SCALING || edit == ROTATION) && selectedBody->isNotPresent())
		return this;

	if (edit == NOEDIT || edit == CAMERA_MOVING)
		this->mousePosition.empty();

	this->edit = edit;
	this->mousePosition = position;
	log();
	return this;
}

inline EditManager* EditManager::setEdit(char edit, vec2 position)
{
	return setEdit(Edit(toupper(edit)), position);
}

inline EditManager* EditManager::setAxe(Axe axe)
{
	switch (axe) 
	{
	case ALL:
		this->axe = ALL;
		break;
	case NOAXE:
		this->axe = NOAXE;
		if (isEdit(CAMERA_MOVING))
			setEdit(NOEDIT);
		break;
	case X:
	case Y:
	case Z:
		this->axe = (isAxe(ALL)) ? ALL : axe;
		if (isEdit(NOEDIT))
			setEdit(CAMERA_MOVING);
		break;
	default:
		break;
	}

	log();
	return this;
}

inline EditManager * EditManager::setAxe(char axe)
{
	return setAxe(Axe(toupper(axe)));
}

inline EditManager* EditManager::setCheckpoint()
{
	if (backupBody.isPresent())
		delete backupBody.get();
	selectedBody->isPresent() ? backupBody.set(new RigidBody(*selectedBody->getElement())) : backupBody.empty();
	return this;
}

inline EditManager* EditManager::backup()
{
	if (selectedBody->isPresent() && backupBody.isPresent())
	{
		selectedBody->getElement()->setPosition(backupBody.get()->getPosition());
		selectedBody->getElement()->setScale(backupBody.get()->getScale());
		selectedBody->getElement()->setAngles(backupBody.get()->getAngles());
	}
	return this;
}

inline bool EditManager::isEdit(Edit edit)
{
	return this->edit == edit;
}

inline bool EditManager::isEdit(char edit)
{
	return this->edit == Edit(toupper(edit));
}

inline bool EditManager::isAxe(Axe axe)
{
	return this->axe == axe;
}

inline bool EditManager::isAxe(char axe)
{
	return this->axe == Axe(toupper(axe));
}

inline bool EditManager::isNotEdit(Edit edit)
{
	return !isEdit(edit);
}

inline bool EditManager::isNotEdit(char edit)
{
	return !isEdit(edit);
}

inline bool EditManager::isNotAxe(Axe axe)
{
	return !isAxe(axe);
}

inline bool EditManager::isNotAxe(char axe)
{
	return !isAxe(axe);
}

inline bool EditManager::onObjectEditing()
{
	return isEdit(TRASLATION) || isEdit(SCALING) || isEdit(ROTATION);
}

inline bool EditManager::onAxeEnabled()
{
	return isAxe(X) || isAxe(Y) || isAxe(Z);
}

inline Vector EditManager::getValueOnAxe(double value)
{
	return EditManager::getValueOnAxe(value, axe);
}

inline void EditManager::log()
{
	system("cls");
	cout << "Edit Mode: " << editString() << endl;
	cout << "Enabled Axe: " << axeString() << endl;
}

inline string EditManager::editString()
{
	switch (edit) {
	case NOEDIT:
		return "None";
	case CAMERA_MOVING:
		return "Moving Camera";
	case CAMERA_FIXING:
		return "Fixing Camera";
	case TRASLATION:
		return "Traslation";
	case SCALING:
		return "Scaling";
	case ROTATION:
		return "Rotation";
	default:
		return "Unknown";
	}
}

inline string EditManager::axeString()
{
	switch (axe) {
	case NOAXE:
		return "None";
	case X:
		return "X";
	case Y:
		return "Y";
	case Z:
		return "Z";
	case ALL:
		return "All";
	default:
		return "Unknown";
	}
}
