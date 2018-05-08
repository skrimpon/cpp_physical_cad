/*
 * Panagiotis Skrimponis
 */
#ifndef __Geometry__
#define __Geometry__

template <class T>
class Point {
public:
	Point();					// Constructor.
	Point(const T&, const T&);	// Constructor.
	~Point();					// Destructor.
	inline const T& x() const;	// Query x-coordinate.
	inline const T& y() const;	// Query y-coordinate.
//	inline void draw()  const;	// Draw point in GTK+2.0, with Cairo.
private:
	T _x,						// x-coordinate.
	  _y;						// y-coordinate.
};
// Constructor.
template <class T>
Point<T>::Point(): _x(0), _y(0)
{
}

// Constructor.
template <class T>
Point<T>::Point(const T& x, const T& y): _x(x), _y(y)
{
}
// Destructor.
template <class T>
Point<T>::~Point() {
}

// Function: x
// Query the x value.
template <class T>
inline const T& Point<T>::x() const {
	return _x;
}

// Function: y
// Query the y value.
template <class T>
inline const T& Point<T>::y() const {
	return _y;
}

#endif 