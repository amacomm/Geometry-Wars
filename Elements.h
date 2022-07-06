#pragma once
#include <math.h>
#include "BMP.h"
#include <vector>

class Direct {
public:
	float x, y;
	Direct();
	Direct(float t1, float t2);
	Direct(float t);
	void Norm();
	Direct operator+(Direct d);
};

class Coord {
public:
	float x, y;
	Coord(float t1 = 0, float t2 = 0);
	Coord operator+ (Coord const coord);
	Coord operator- (Coord const coord);
	Coord operator+ (Coord const* coord);
	Coord operator- (Coord const* coord);
	operator Direct() const;
	Coord operator*(float a);
	Coord operator*=(float a);
};

class Colour {
public:
	unsigned char r, g, b;
	Colour(unsigned char t1 = 0, unsigned char t2 = 0, unsigned char t3 = 0);
	operator uint32_t() const;
};

struct Shapes
{
	static std::vector<Coord> ship(float a = 1)
	{
		return { Coord(0, 1.732 / 3) * a, Coord(1.732 / 3,-1.732 / 6) * a,Coord(0,-1.732 / 12) * a, Coord(-1.732 / 3,-1.732 / 6) * a };
	}
	static std::vector<Coord> weapon(float a = 1)
	{
		return { Coord(0, 1.732 / 3) * a, Coord(1.732 / 3,-1.732 / 6) * a, Coord(-1.732 / 3,-1.732 / 6) * a };
	}
	static std::vector<Coord> enemy(float a = 1)
	{
		return { Coord(0.5,0.5) * a, Coord(0.5,-0.5) * a, Coord(-0.5,-0.5) * a, Coord(-0.5,0.5) * a };
	}
};

void loader(unsigned int** image, char* name, int w, int h);

std::vector<unsigned int**> ABCc();