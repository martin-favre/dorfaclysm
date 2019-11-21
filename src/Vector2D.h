#pragma once
#include <SDL_rect.h>
#include <iostream>
class Vector2DInt;
class Vector2D {
public:
	Vector2D();
	Vector2D(double x, double y=0);
	Vector2D(const Vector2DInt & vec);
	Vector2D & operator+=(const Vector2D &);
	bool operator==(const Vector2D&)const;
	Vector2D operator-(const Vector2D & other)const;
	Vector2D operator+(const Vector2D & other)const;
	
	/*-------------------------------------------------------
	Returns the angle of the vector in degrees
	(1,0)->0
	(0,1)->90
	---------------------------------------------------------*/

	static double angle(const Vector2D& vec);
	template <typename multiplier_type>
	Vector2D operator*(const multiplier_type& value)const;
	template <typename multiplier_type>
	Vector2D operator/(const multiplier_type& value)const;
	template <typename multiplier_type>
	Vector2D& operator/=(const multiplier_type& value);
	double magnitude()const;
	double magnitude_squared()const;
	double dot(const Vector2D& other)const;
	void rotate_around(double angle, Vector2D point);
	void rotate(double angle);
	bool equals(const Vector2D & other, double accuracy);
	/*-------------------------------------------------------
	(x,y) -> (-y,x)
	---------------------------------------------------------*/
	Vector2D perpendicular()const;
	Vector2D normalized()const;
	void set(double, double);
	void round_to_int();
	SDL_Point get_sdl_point();
	double x = 0;
	double y = 0;
	friend std::ostream & operator <<(std::ostream & stream, const Vector2D &);
private:
	
};

template<typename multiplier_type>
inline Vector2D Vector2D::operator*(const multiplier_type & value)const
{	
	return Vector2D(x*value, y*value);
}

template<typename multiplier_type>
inline Vector2D Vector2D::operator/(const multiplier_type& value)const
{
	return Vector2D(x / value, y / value);
}
template<typename multiplier_type>
inline Vector2D& Vector2D::operator/=(const multiplier_type& value)
{
	x /= value;
	y /= value;
	return *this;
}
