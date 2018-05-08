/*
 * Panagiotis Skrimponis
 */

#ifndef __PIN__
#define __PIN__
#include "headerdef.h"
// Pin Class.
class Pin
{
public:
	Pin(const std::string&, const Point <float>&);				// Constructor.
	~Pin();														// Destructor.
    inline const std::string& name() const;						// Query the Pin name.
    inline const Point <float>& location() const;				// Query the Pin location.
    inline const std::vector<std::string>& connection() const;	// Query the Pin connection.
    inline void add_connection(const std::string&);				// Add a connection.
private:
	std::string _name;											// Pin name.
	Point <float> _location;									// Pin location.
	std::vector <std::string> _connected_components;			// Pin connected components.
};

inline const std::string& Pin::name() const
{
	return _name;
}

inline const Point <float>& Pin::location() const
{
	return _location;
}

inline void Pin::add_connection(const std::string& component)
{
	_connected_components.push_back(component);
}

inline const std::vector<std::string>& Pin::connection() const
{
	return _connected_components;
}
#endif