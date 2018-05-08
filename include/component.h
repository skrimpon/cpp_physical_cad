/*
 * Panagiotis Skrimponis
 */

#ifndef __COMPONENT__
#define __COMPONENT__
#include "headerdef.h"
// Component Class.
class Component
{
public:
	Component(const std::string&);								// Constructor.
	~Component();												// Destructor.
    inline const std::string& name() const;						// Query the Component name.
    inline const Point <float>& location() const;				// Query the Pin location.
    inline const std::vector<std::string>& connection() const;	// Query the Component connection.
    inline void add_connection(const std::string&);				// Add a connection.
    inline void set_location(const Point <float>&);				// Set Component location.
    inline const float R() const;
    inline const float G() const;
    inline const float B() const;
private:
	float _rgb[3];
	std::string _name;											// Component name.
	Point <float> _location;									// Component location.
	std::vector <std::string> _connected_components;			// Component connected components.
};

inline const std::string& Component::name() const
{
	return _name;
}

inline const Point <float>& Component::location() const
{
	return _location;
}

inline void Component::add_connection(const std::string& component)
{
	_connected_components.push_back(component);
}

inline const std::vector<std::string>& Component::connection() const
{
	return _connected_components;
}

inline void Component::set_location(const Point <float>& location)
{
	_location = location;
}

inline const float Component::R() const
{
	return _rgb[0];
}
inline const float Component::G() const
{
	return _rgb[1];
}
inline const float Component::B() const
{
	return _rgb[2];
}

#endif