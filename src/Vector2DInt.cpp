#include "Vector2DInt.h"
#include "Vector2D.h"
#include "Helpers.h"

Vector2DInt::Vector2DInt() {}
Vector2DInt::Vector2DInt(int x_, int y_) :x(x_), y(y_) {}
Vector2DInt::Vector2DInt(float x_, float y_) :x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}
Vector2DInt::Vector2DInt(double x_, double y_) :x(Helpers::roundToInt(x_)), y(Helpers::roundToInt(y_)) {}
Vector2DInt::Vector2DInt(const Vector2D & vec) : x(Helpers::roundToInt(vec.x)), y(Helpers::roundToInt(vec.y)) {}

bool Vector2DInt::operator<(const Vector2DInt & left)const
{
	if (x != left.x) 
	{
		return x < left.x;
	}
	else
	{
		return y < left.y;
	}

}


bool Vector2DInt::operator!=(const Vector2DInt & other)const
{
	return !(*this == other);
}

bool Vector2DInt::operator==(const Vector2DInt & other)const
{
	return (other.y == y) && (other.x == x);
}
Vector2DInt Vector2DInt::operator*(const int & val)
{
	return Vector2DInt(x*val, y*val);
}
Vector2DInt& Vector2DInt::operator+=(const Vector2DInt & other)
{
	x += other.x;
	y += other.y;
	return *this;
}