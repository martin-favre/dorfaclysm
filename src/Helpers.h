#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned int uint;

/*-------------------------------------------------------
Terminates if condition is false. Turn off with DEBUG flag.
Outside namespace for convenience.
---------------------------------------------------------
@param condition - asserts if false
@param message - Gets logged
---------------------------------------------------------*/

//#define ASSERT(condition, message) do {} while (false)
#ifndef DEBUG
inline void ASSERT(bool condition, const std::string& message){
	(void) condition;
	(void) message;
}
#else
#define ASSERT(condition, message) \
   do { \
       if (! (condition)) { \
           Logging::log(std::stringstream() << "std::endl" <<  "Assertion: " #condition << std::endl << \
           "Failed in file: " << __FILE__  << std::endl << \
           "line: " << __LINE__ << std::endl << \
           "Message: " << message << std::endl << std::endl); \
           std::terminate(); \
       } \
   } while (false)
#endif

class Vector2DInt;
namespace Helpers {
	/*-------------------------------------------------------
		Get random int.
	---------------------------------------------------------
	@param min, inclusive
	@param max, inclusive
	@return result
	---------------------------------------------------------*/
	
	int random_int(int min, int max);

	/*-------------------------------------------------------
		Extracts the filename from path.
	---------------------------------------------------------
		C:\path\to\file.exe -> file.exe
	@param path
	@return filename
	---------------------------------------------------------*/
	
	std::string get_filename_from_path(const std::string & path);

	/*-------------------------------------------------------
		Rounds floating point value to integer. 
	---------------------------------------------------------
		0.499 -> 0
		0.51111 -> 1
		0.5 -> 1
	@param v - floating point value
	@return closest integer.
	---------------------------------------------------------*/
	int round_to_int(float v);
	int round_to_int(double v);

	/*-------------------------------------------------------
	Takes a vector and add the second argument to all elements.
	---------------------------------------------------------
	The type needs to support +=
	---------------------------------------------------------*/
	template<typename T, typename Allocator>
	void add_to_all(std::vector<T, Allocator>& vec, const T& to_add)
	{
		for (size_t i = 0; i < vec.size(); ++i)
		{
			vec[i] += to_add;
		}
	}

	/*-------------------------------------------------------
		Returns the squared distance between two positions
	---------------------------------------------------------
		d = (bx-ax)^2 + (by-ay)^2
	@param a - one of the positions
	@param b - the other position
	@return - distance between the two positions
	---------------------------------------------------------*/
	
	double get_squared_distance_between_positions(const Vector2DInt & a, const Vector2DInt & b);

	/*-------------------------------------------------------
	Parameters
	---------------------------------------------------------*/
	const double MATH_PI = 3.14159265358979323846264338327950288;
	const double TO_RAD = MATH_PI / 180;
}