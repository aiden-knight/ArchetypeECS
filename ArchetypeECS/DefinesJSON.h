#pragma once
#include <JSON/nlohmann/json.hpp>

#pragma region Structures_H
#include "Structures.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Colour, r, g, b, a);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FVector2, x, y);

#pragma endregion Structures_H

#pragma region SDL_H
#include "Window.h"

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowData, title, xPosition, yPosition, width, height, flags);

#pragma endregion SDL_H