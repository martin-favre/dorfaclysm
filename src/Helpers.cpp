#include "Helpers.h"

#include <random>

#include "Vector2DInt.h"

namespace Helpers {
static std::random_device random_int_rd;
static std::mt19937 random_int_rng(random_int_rd());

int randomInt(int min, int max) {
  std::uniform_int_distribution<int> uni(min, max);
  return uni(random_int_rng);
}

int roundToInt(float v) { return int(v + 0.5); }

int roundToInt(double v) { return int(v + 0.5); }

double getSquaredPositionBetweenPositions(const Vector2DInt& a,
                                          const Vector2DInt& b) {
  const double xdiff = b.x - a.x;
  const double ydiff = b.y - a.y;
  return xdiff * xdiff + ydiff * ydiff;
}

std::string getFilenameFromPath(const std::string& path) {
  std::size_t found = path.find_last_of("/\\");
  if (found == std::string::npos) {
    return "";
  } else {
    return path.substr(found + 1);
  }
}

}  // namespace Helpers