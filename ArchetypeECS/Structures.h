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

struct Health
{
	int value = 100;
};
struct Mana
{
	int value = 0;
};