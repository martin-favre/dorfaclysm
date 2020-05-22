#pragma once
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <functional>
typedef unsigned int uint;

/*-------------------------------------------------------
Terminates if condition is false. Turn off with DEBUG flag.
Outside namespace for convenience.
---------------------------------------------------------
@param condition - asserts if false
@param message - Gets logged
---------------------------------------------------------*/

//#define ASSERT(condition, message) do {} while (false)

#ifdef VSCODE  // vscode can't handle macros, so just hide in the editor
inline void ASSERT(bool condition, const std::string& message) {
  (void)condition;
  (void)message;
}
#else
#ifndef DEBUG
inline void ASSERT(bool condition, const std::string& message) {
  (void)condition;
  (void)message;
}
#else
void assertLog(const std::string& message, const std::string& file,
               const int line);
#define ASSERT(condition, message)            \
  do {                                        \
    if (!(condition)) {                       \
      assertLog(message, __FILE__, __LINE__); \
      assert(false);                          \
    }                                         \
  } while (false);
#endif
#endif

namespace Helpers {

/*-------------------------------------------------------
        Get random int.
---------------------------------------------------------
@param min, inclusive
@param max, inclusive
@return result
---------------------------------------------------------*/

int randomInt(int min, int max);

/*-------------------------------------------------------
        Extracts the filename from path.
---------------------------------------------------------
        C:\path\to\file.exe -> file.exe
@param path
@return filename
---------------------------------------------------------*/

std::string getFilenameFromPath(const std::string& path);

/*-------------------------------------------------------
        Rounds floating point value to integer.
---------------------------------------------------------
        0.499 -> 0
        0.51111 -> 1
        0.5 -> 1
@param v - floating point value
@return closest integer.
---------------------------------------------------------*/
template <typename T>
constexpr int roundToInt(T v) {
  return int(v + 0.5);
}

/*-------------------------------------------------------
Takes a vector and add the second argument to all elements.
---------------------------------------------------------
The type needs to support +=
---------------------------------------------------------*/
template <typename T, typename Allocator>
void addToAll(std::vector<T, Allocator>& vec, const T& to_add) {
  for (size_t i = 0; i < vec.size(); ++i) {
    vec[i] += to_add;
  }
}

template <typename T, typename Allocator>
void eraseAll(std::list<T, Allocator>& container, T itemToRemove) {
  container.erase(std::remove(container.begin(), container.end(), itemToRemove),
                  container.end());
}

template <typename T, typename Allocator>
void eraseAll(std::list<T, Allocator>& container, std::function<bool(const T&)> foo) {
  container.erase(std::remove_if(container.begin(), container.end(), foo),
                  container.end());
}

/*-------------------------------------------------------
        Returns the squared distance between two positions
---------------------------------------------------------
        d = (bx-ax)^2 + (by-ay)^2
@param a - one of the positions
@param b - the other position
@return - distance between the two positions
---------------------------------------------------------*/
template <typename T>
constexpr double getSquaredPositionBetweenPositions(const T& a, const T& b) {
  const double xdiff = b.x - a.x;
  const double ydiff = b.y - a.y;
  return xdiff * xdiff + ydiff * ydiff;
}

/*-------------------------------------------------------
Parameters
---------------------------------------------------------*/
constexpr double MATH_PI = 3.14159265358979323846264338327950288;
constexpr double TO_RAD = MATH_PI / 180;
}  // namespace Helpers