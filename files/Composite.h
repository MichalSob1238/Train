#pragma once
#include "Object.h"
#include "Shape.h"
#include <memory>

class Composite : public Object
{
protected:
	std::vector<Shape*> elements;
public:
	/* create empty composite object at given location */
	Composite(const glm::vec3& positon);

	/* draw composite elements in regard to it's position */
	void draw();

	void setShader(const ShaderProgram& shader);
	
	/* add element to composite */
	void addElement( Shape& object);

	/* remove element from composite */
	void removeElement(uint32_t index);

	/* get pointer to element in composite at given index, returns elements.end() if object doesn't exist */
	Shape* getElement(int index);

};

