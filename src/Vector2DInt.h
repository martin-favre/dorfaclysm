#pragma once
#include <cinttypes>
#include <cmath>
/*-------------------------------------------------------
	Contains 2D vector of integers	
---------------------------------------------------------*/

class Vector2D;
class Vector2DInt
{
public:
	Vector2DInt();
	Vector2DInt(int x_, int y_);

	/*-------------------------------------------------------
	Values are rounded.
	---------------------------------------------------------*/
	Vector2DInt(float x_, float y_);

	/*-------------------------------------------------------
	Values are rounded.
	---------------------------------------------------------*/
	Vector2DInt(double x_, double y_);

	/*-------------------------------------------------------
	Values are rounded.
	---------------------------------------------------------*/
	Vector2DInt(const Vector2D & vec);

	/*-------------------------------------------------------
		Note: designed for creating an unique value for usage in a map,
		not checking if one is less than the other. Since that doesn't 
		sense.
	---------------------------------------------------------*/
	bool operator<(const Vector2DInt & left)const;
	
	bool operator!=(const Vector2DInt & other)const;
	bool operator==(const Vector2DInt & other)const;
	Vector2DInt operator*(const int & val);
	Vector2DInt& operator+=(const Vector2DInt & other);
	int x = 0;
	int y = 0;
};