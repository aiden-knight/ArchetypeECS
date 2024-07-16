#pragma once
#include "JSON/nlohmann/json.hpp"
#include <stdint.h>

typedef uint8_t Uint8;

struct Colour
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Colour, r, g, b, a);

struct FVector2
{
	float x;
	float y;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FVector2, x, y);