#include "Elements.h"

Direct::Direct() :x(0), y(1) {}
Direct::Direct(float t1, float t2) : x(t1), y(t2) {
	if (x != 0 && y != 0)
		Norm();
}
Direct::Direct(float t) {
	x = cos(t);
	y = sin(t);
	Norm();
}
void Direct::Norm() {
	float t = sqrt(x * x + y * y);
	x /= t;
	y /= t;
}
Direct Direct::operator+(Direct d) {
	return Direct(x + d.x, y + d.y);
}

Coord::Coord(float t1, float t2) : x(t1), y(t2) {}
Coord Coord::operator+ (Coord const coord) {
	return Coord(x + coord.x, y + coord.y);
}
Coord Coord::operator- (Coord const coord) {
	return Coord(x - coord.x, y - coord.y);
}
Coord Coord::operator+ (Coord const* coord) {
	return Coord(x + coord->x, y + coord->y);
}
Coord Coord::operator- (Coord const* coord) {
	return Coord(x - coord->x, y - coord->y);
}
Coord::operator Direct() const {
	return Direct(this->x, this->y);
}
Coord Coord::operator*(float a) {
	return Coord(x * a, y * a);
}
Coord Coord::operator*=(float a) {
	this->x *= a;
	this->y *= a;
	return *this;
}

Colour::Colour(unsigned char t1, unsigned char t2, unsigned char t3) :r(t1), g(t2), b(t3) {}
Colour::operator uint32_t() const {
	return (r << 16) + (g << 8) + b;
}

void loader(unsigned int** image, char* name, int w, int h) {
	unsigned char* charImage;
	charImage = BMP24_read(h, w, name);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			image[h - i - 1][j] = Colour(charImage[(j + w * i) * 3+2], charImage[(j + w * i) * 3 + 1], charImage[(j + w * i) * 3]);
		}
	}
	delete[] charImage;
}

std::vector<unsigned int**> ABCc() {
	std::vector<unsigned int**> alf;
	unsigned int** img;
	img = new unsigned int* [80];
	for (int i = 0; i < 80; i++)
		img[i] = new unsigned int[170];
	loader(img, (char*)"picture/ABC.bmp", 170, 80);
	for (int i = 0; i < 36; i++) {
		alf.push_back(new unsigned int* [20]);
		for (int j = 0; j < 20; j++) {
			alf[i][j] = new unsigned int[17];
			for (int k = 0; k < 17; k++) {
				alf[i][j][k] = img[j + 20 * (i / 10)][k + 17 * (i % 10)];
			}
		}
	}
	for (int i = 0; i < 80; i++)
		delete[] img[i];
	delete[] img;
	return alf;
}