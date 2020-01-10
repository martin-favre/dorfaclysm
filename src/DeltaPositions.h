#pragma once
#include "Vector3DInt.h"
// constexpr Vector3DInt DELTA_POSITIONS[]{
//     {-1, -1, -1}, {0, -1, -1}, {1, -1, -1}, {-1, 0, -1}, {0, 0, -1},
//     {1, 0, -1},   {-1, 1, -1}, {0, 1, -1},  {1, 1, -1},  {-1, -1, 0},
//     {0, -1, 0},   {1, -1, 0},  {-1, 0, 0},  {1, 0, 0},   {-1, 1, 0},
//     {0, 1, 0},    {1, 1, 0},   {-1, -1, 1}, {0, -1, 1},  {1, -1, 1},
//     {-1, 0, 1},   {0, 0, 1},   {1, 0, 1},   {-1, 1, 1},  {0, 1, 1},
//     {1, 1, 1}};

// don't use the diagonals for now
constexpr Vector3DInt DELTA_POSITIONS[]{
    {0, 0, -1}, {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
};

constexpr Vector3DInt DELTA_POSITIONS_HORIZONTAL[]{
    {0, -1, 0}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}};
