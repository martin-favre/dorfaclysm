#pragma once
#include <string>

#include "Vector2DInt.h"
namespace Paths {
const std::string UBUNTU_FONT = "assets/fonts/Ubuntu-B.ttf";

const std::string GRASS_TILE = "assets/sprites/Grassland@64x64.png";
constexpr Vector2DInt SIZE_OF_GRASS_TILE{64, 64};

const std::string NPC_TILE = "assets/sprites/npc5.png";
constexpr Vector2DInt SIZE_OF_NPC_TILE{26, 36};

const std::string BROWN_SQUARE = "assets/sprites/BrownSquare64x64.png";
constexpr Vector2DInt SIZE_OF_BROWN_SQUARE{64, 64};

const std::string DF_TILE = "assets/sprites/df_items24x24.png";
constexpr Vector2DInt SIZE_OF_DF_TILE{24, 24};

const std::string RG_TILE_COLORED = "assets/sprites/colored.png";
constexpr Vector2DInt SIZE_OF_RG_TILE_COLORED{16, 16};

const std::string RG_TILE_TRANSPARENT = "assets/sprites/colored_transparent.png";
constexpr Vector2DInt SIZE_OF_RG_TILE_TRANSPARENT{16, 16};

}  // namespace Paths