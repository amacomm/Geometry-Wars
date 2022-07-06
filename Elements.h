#pragma once
#include <math.h>
#include "BMP.h"

class Direct {
public:
	float x, y;
	Direct() :x(0), y(1) {}
	Direct(float t1, float t2) : x(t1), y(t2) {
		if (x == 0 && y == 0){
			x = -1;
			y = -1;
		}
		Norm();
	}
	Direct(float t) {
		x = cos(t);
		y = sin(t);
		Norm();
	}
	void Norm() {
		float t = sqrt(x * x + y * y);
		x /= t;
		y /= t;
	}
};

class Coord {
public:
	float x, y;
	Coord(float t1 = 0, float t2 = 0) : x(t1), y(t2) {}
	Coord operator+ (Coord const coord) {
		return Coord(x + coord.x, y + coord.y);
	}
	Coord operator- (Coord const coord) {
		return Coord(x - coord.x, y - coord.y);
	}
	Coord operator+ (Coord const* coord) {
		return Coord(x + coord->x, y + coord->y);
	}
	Coord operator- (Coord const* coord) {
		return Coord(x - coord->x, y - coord->y);
	}
	operator Direct() const {
		return Direct(this->x, this->y);
	}
	Coord operator*(float a) {
		return Coord(x * a, y * a);
	}
	Coord operator*=(float a) {
		this->x *= a;
		this->y *= a;
		return *this;
	}
};

class Colour {
public:
	unsigned char r, g, b;
	operator uint32_t() const {
		return (r << 16) + (g << 8) + b;
	}
	Colour(unsigned char t1 = 0, unsigned char t2 = 0, unsigned char t3 = 0) :r(t1), g(t2), b(t3) {}
};

struct Shapes
{
	static std::vector<Coord> ship(float a = 1)
	{
		return { Coord(0, 1.732 / 3) * a, Coord(1.732 / 3,-1.732 / 6) * a, Coord(-1.732 / 3,-1.732 / 6) * a };
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

void loader(unsigned int** image, char* name, int w, int h) {
	unsigned char* charImage;
	charImage = BMP24_read(h, w, name);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			image[h - i - 1][j] = Colour(charImage[(j + w * i) * 3], charImage[(j + w * i) * 3 + 1], charImage[(j + w * i) * 3 + 2]);
		}
	}
	delete[] charImage;
}
