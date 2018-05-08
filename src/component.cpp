#include "component.h"

Component::Component(const std::string& name) : _name(name)
{
	_rgb[0] = (float)(rand())/RAND_MAX;
	_rgb[1] = (float)(rand())/RAND_MAX;
	_rgb[2] = (float)(rand())/RAND_MAX;
}

Component::~Component()
{
	
}