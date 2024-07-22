#pragma once
#include <stdint.h>

typedef uint8_t Uint8;

struct Colour
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

struct FVector2
{
	float x;
	float y;
};

struct Position
{
	FVector2 value{ 0, 0 };
};

struct Extents
{
	FVector2 value{ 0, 0 };
};

struct Rect
{
	Position pos;
	Extents extents;
	Colour colour;
};