#pragma once
#include "Engine.h"
#include <ctime>
#include <windows.h>
#include "Elements.h"

class Explosion {
	Coord _coord;
	int _count;
	std::vector<Coord> _dots;
	std::vector<Direct> _direct;
	std::vector<Colour> _colour;
	std::vector<unsigned char> _speed;
	int life;
public:
	friend void Draw(uint32_t(*image)[SCREEN_WIDTH], Explosion& e);
	Explosion(Coord coord=Coord()): _coord(coord) {
		life = 0;
		srand(time(0));
		_count = rand() % 100;
		for (int i = 0; i < _count; ++i) {
			_dots.push_back(coord);
			_direct.push_back(Direct(rand() % 100 - 50, rand() % 100 - 50));
			_colour.push_back(rand() % 2 ? Colour(255, 51, 51) : Colour(255, 216, 138));
			_speed.push_back(rand() % 10 + 10);
		}
	}

	bool MakeMove() {
		for (int i = 0; i < _count; i++) {
			_dots[i].x += _direct[i].x /_speed[i];
			_dots[i].y += _direct[i].y /_speed[i];
		}
		return (++life < 900 && !TouchedFrame()) ? false : true;
	}
	bool TouchedFrame() {
		return TouchedFrameX() || TouchedFrameY();
	}
	bool TouchedFrameX() {
		for (int i = 0; i < _dots.size(); i++)
			if (_dots[i].x <= 0 || _dots[i].x >= SCREEN_WIDTH) return true;
		return false;
	}
	bool TouchedFrameY() {
		for (int i = 0; i < _dots.size(); i++)
			if (_dots[i].y <= 0 || _dots[i].y >= SCREEN_HEIGHT) return true;
		return false;
	}
};
extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Explosion& e) {
	for (int i = 0; i < e._count; ++i) {
		if (!e.TouchedFrame())
			try {
			if ((e._dots[i].y != e._dots[i].y) || (e._dots[i].x != e._dots[i].x))
				throw 1;
			image[(int)(e._dots[i].y)][(int)(e._dots[i].x)] = e._colour[i];
		}
		catch (...) {

		}
	}
}

class Object {
protected:
	float _speed;
	std::vector<Coord> _dots;
	Coord _coord;
	Direct _direct;
	Colour _colour;
	virtual void Moving(Coord coord = Coord()) {
		_coord.x += _direct.x*_speed;
		_coord.y -= _direct.y*_speed;
	}

public:
	friend void Draw(uint32_t(*image)[SCREEN_WIDTH], Object&, unsigned int**);
	virtual void Rotate() {
		for (int i = 0; i < _dots.size(); ++i) {
			_dots[i].x *= cos(_direct.x);
			_dots[i].y *= sin(_direct.y);
		}
	}
	virtual void Rotate(float fi) {
		for (int i = 0; i < _dots.size(); ++i) {
			float x = _dots[i].x * cos(fi) + _dots[i].y * sin(fi);
			float y = _dots[i].y * cos(fi) - _dots[i].x * sin(fi);
			_dots[i].x =x;
			_dots[i].y =y;
		}
	}
	Coord getCoord() {
		return _coord;
	}
	Direct getDirect() {
		return _direct;
	}
	bool TouchedFrame() {
		return TouchedFrameX() || TouchedFrameY();
	}
	bool TouchedFrameX() {
		for (int i = 0; i < _dots.size(); i++)
			if (_dots[i].x + _coord.x <= 0 || _dots[i].x + _coord.x >= SCREEN_WIDTH) return true;
		return false;
	}
	bool TouchedFrameY() {
		for (int i = 0; i < _dots.size(); i++)
			if (_dots[i].y+_coord.y <= 0 || _dots[i].y + _coord.y >= SCREEN_HEIGHT) return true;
		return false;
	}
	bool IsIncluded(Coord point) {
		std::vector<float> lambda(_dots.size());
		bool l = false;
		for (int t = 0; t < _dots.size() - 2; t++) {
			bool s = true;
			int d[] = { 0, 1 + t, 2 + t };
			for (int i = 0; i < 3; i++)
			{
				int o = (0 - 1) % 3;
				int k1 = d[i], k2 = d[(i + 1) % 3], k0 = d[(i - 1) % 3 >=0 ? (i - 1) % 3 : (i - 1) % 3+3];
				lambda[i] = ((_dots[k1].x - _dots[k2].x) * (point.y - _dots[k2].y-_coord.y) - (_dots[k1].y - _dots[k2].y) * (point.x - _dots[k2].x - _coord.x)) / ((_dots[k1].x - _dots[k2].x) * (_dots[k0].y - _dots[k2].y) - (_dots[k1].y - _dots[k2].y) * (_dots[k0].x - _dots[k2].x));
				s = s && (lambda[i] >= 0);
			}
			l = l || s;
		}
		return l;
	}
};
extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Object &o, unsigned int** img = nullptr) {
	if(!img)
	for (int i = 0; i < o._dots.size(); i++) {
		int x0 = -o._dots[i].x + o._coord.x,
			y0 = -o._dots[i].y + o._coord.y,
			x1 = (-o._dots[(i + 1) % o._dots.size()].x) + o._coord.x,
			y1 = (-o._dots[(i + 1) % o._dots.size()].y) + o._coord.y;
		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		int dx = x1 - x0;
		int dy = y1 - y0;
		float derror = std::abs(dy / float(dx));
		float error = 0;
		int y = y0;
		for (int x = x0; x <= x1; x++) {
			if (!(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_WIDTH))
			{
				if (steep) image[x][y] = o._colour;
				else image[y][x] = o._colour;
			}
			error += derror;
			if (error > .5) {
				y += (y1 > y0 ? 1 : -1);
				error -= 1.;
			}
		}
	}
	else {
		std::vector<float> lambda(3);
		std::vector<Coord> texture = { Coord(21, 0), Coord(44,33) ,Coord(28, 21), Coord(0,33) };
		//std::vector<Coord> texture = { Coord(0, 0), Coord(49,0) ,Coord(49, 50), Coord(0,49)};
		bool s = true;
		for (int t = 0; t < o._dots.size() - 2; t++) {
			int d[] = { 0, 1 + t, 2 + t };
			int xmax = (int)o._coord.x - (o._dots[d[0]].x > o._dots[d[1]].x ? (o._dots[d[2]].x > o._dots[d[1]].x ? o._dots[d[1]].x : o._dots[d[2]].x) : (o._dots[d[2]].x > o._dots[d[0]].x ? o._dots[d[0]].x : o._dots[d[2]].x));
			int ymax = (int)o._coord.y - (o._dots[d[0]].y > o._dots[d[1]].y ? (o._dots[d[2]].y > o._dots[d[1]].y ? o._dots[d[1]].y : o._dots[d[2]].y) : (o._dots[d[2]].y > o._dots[d[0]].y ? o._dots[d[0]].y : o._dots[d[2]].y));
			int xmin = (int)o._coord.x - (o._dots[d[0]].x < o._dots[d[1]].x ? (o._dots[d[2]].x < o._dots[d[1]].x ? o._dots[d[1]].x : o._dots[d[2]].x) : (o._dots[d[2]].x < o._dots[d[0]].x ? o._dots[d[0]].x : o._dots[d[2]].x));
			int ymin = (int)o._coord.y - (o._dots[d[0]].y < o._dots[d[1]].y ? (o._dots[d[2]].y < o._dots[d[1]].y ? o._dots[d[1]].y : o._dots[d[2]].y) : (o._dots[d[2]].y < o._dots[d[0]].y ? o._dots[d[0]].y : o._dots[d[2]].y));
			for (int x = xmin; x < xmax; ++x)
				for (int y = ymin; y < ymax; ++y) {
					s = true;
					for (int i = 0; i < 3; i++)
					{
						int k1 = d[i], k2 = d[(i + 1) % 3], k0 = d[(i - 1) % 3 >= 0 ? (i - 1) % 3 : (i - 1) % 3 + 3];
						lambda[i] = ((o._dots[k1].x - o._dots[k2].x) * (-y - o._dots[k2].y + (int)o._coord.y) - (o._dots[k1].y - o._dots[k2].y) * (-x - o._dots[k2].x + (int)o._coord.x)) / ((o._dots[k1].x - o._dots[k2].x) * (o._dots[k0].y - o._dots[k2].y) - (o._dots[k1].y - o._dots[k2].y) * (o._dots[k0].x - o._dots[k2].x));
						s = s && (lambda[i] >= 0);
					}
					if (s)
						if (!(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_WIDTH)) {
							int t1 = texture[d[2]].y * lambda[0] + texture[d[0]].y * lambda[1] + texture[d[1]].y * lambda[2];
							int t2 = texture[d[2]].x * lambda[0] + texture[d[0]].x * lambda[1] + texture[d[1]].x * lambda[2];
							buffer[y][x] = img[t1][t2];//Colour(255, 255, 255);
						}
				}
		}
	}
}

class Weapon : public Object {
public:
	Weapon(Coord coord, Direct direct, Colour colour = Colour(255, 255, 0)) {
		_colour = colour;
		_dots =  Shapes::weapon(SCREEN_HEIGHT / 100);
		_coord = coord;
		_direct = direct;
		_speed = (float)2;
		//PlaySound("shut.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC );
	}
	void MakeMove() {
		Moving();
	}
};

class Ship final: public Object {
	float _speed2 = 0;
	float _boost = (float)1 / 30;
public:
	Ship(Coord coord = Coord(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), Direct direct = Direct(0, 1), Colour colour = Colour(255, 255, 255)){
		float a=SCREEN_HEIGHT / 20;
		_colour = colour;
		_dots = Shapes::ship();
		for (int i = 0; i < _dots.size(); _dots[i++] *= a);
		_coord = coord;
		_speed = 0;
		_direct = direct;
	}
	void ChangeDirection(int x, int y) {
		_direct = Direct(x - _coord.x, y - _coord.y);
		Rotate();
	}
	void Moving(Coord c=Coord()) override {
		_coord.x += _speed;
		_coord.y += _speed2;
		if (TouchedFrame()) {
			_speed *= TouchedFrameX() ? -1 : 1;
			_speed2 *= TouchedFrameY() ? -1 : 1;
		}
	}
	void MakeMove(float dt) {
		_speed += _boost*_direct.x*dt*30;
		_speed2 += _boost*_direct.y * dt*30;
		Moving();
	}
	void Rotate() override {
		_dots = Shapes::ship(SCREEN_HEIGHT / 20);
		Roll();
		float t = -atan2(_direct.y, _direct.x) - 3.1416 / 2;
		for (int i = 0; i < _dots.size(); i++)
		{
			float x = _dots[i].x * cos(t) + _dots[i].y * sin(t);
			float y = _dots[i].y * cos(t) - _dots[i].x * sin(t);
			_dots[i].x = x;
			_dots[i].y = y;
		}
	}
	void Braking(float dt) {
		_speed += _boost/2 * (_speed > 0 ? -1 : 1)*dt*30;
		_speed2 += _boost/2 * (_speed2 > 0 ? -1 : 1)*dt*30;
		Moving();
	}
	Coord getShipNose() {
		return _dots[0];
	}
	void Roll() {
		Direct roll = _direct +Direct(_speed, _speed2);
		float t = -atan2(roll.y, roll.x) - 3.1416 / 2;
		for (int i = 1; i < _dots.size(); i+=2)
		{
			float x = _dots[i].x * cos(t) + _dots[i].y * sin(t);
			float y = _dots[i].y * cos(t) - _dots[i].x * sin(t);
			_dots[i].x = x;
			_dots[i].y = y;
		}
	}
	void Deth() {
		_coord = Coord(-100, -100);
	}
};

class Enemy: public Object {
protected:
	char _life;
	float _turnÑounter = 0;
	float _lastDamage=1;
	Colour _colorStabile;
public:
	bool ChackDamage(Coord point, float db) {
		if (IsIncluded(point)) {
			_lastDamage *= _lastDamage > 0.25 ? 0 : 1;
			_life -= (_lastDamage == 0);
		}
		_lastDamage += db;

		return _life==0;
	}
	virtual bool MakeMove(Coord coord = Coord()) = 0;
};
class Create {
	Coord _coord;
	int _count;
	std::vector<Coord> _dots1, _dots2;
public:
	Enemy* enemy;
	Create(Enemy* e = nullptr) : _coord(enemy->getCoord()), enemy(e) {
		srand(time(0));
		_count = rand() % 5 + 5;
		for (int i = 0; i < _count; ++i) {
			Direct d(rand() % 100 - 50, rand() % 100 - 50);
			_dots1.push_back(Coord(_coord.x + d.x * 50, _coord.y + d.y * 50));
			_dots2.push_back(Coord(_coord.x + d.x * 50, _coord.y + d.y * 50));
		}
	}
	void Draw(uint32_t(*image)[SCREEN_WIDTH]) {
		for (int i = 0; i < _count; i++) {
			int x0 = _dots1[i].x,
				y0 = _dots1[i].y,
				x1 = (_dots2[i].x),
				y1 = (_dots2[i].y);
			bool steep = false;
			if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
				std::swap(x0, y0);
				std::swap(x1, y1);
				steep = true;
			}
			if (x0 > x1) {
				std::swap(x0, x1);
				std::swap(y0, y1);
			}
			int dx = x1 - x0;
			int dy = y1 - y0;
			float derror = std::abs(dy / float(dx));
			float error = 0;
			int y = y0;
			for (int x = x0; x <= x1; x++) {
				if (!(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT))
				{
					if (steep) image[x][y] = Colour(255, 255, 255);
					else image[y][x] = Colour(255, 255, 255);
				}
				error += derror;
				if (error > .5) {
					y += (y1 > y0 ? 1 : -1);
					error -= 1.;
				}
			}
		}
	}
	bool MakeMove() {
		for (int i = 0; i < _count; i++) {
			_dots2[i].x += (-_dots2[i].x + _coord.x) / 60;
			_dots2[i].y += (-_dots2[i].y + _coord.y) / 60;
		}
		return (-_dots2[0].x + _coord.x - _dots2[0].y + _coord.y) < 1;
	}
};

class StableEnemy final: public Enemy {
public:
	StableEnemy(Coord coord, Direct direct, int level = 0, Colour colour = Colour(255, 0, 0)) {
		_life = 5;
		_speed = 0.25+level*0.125;
		_colorStabile= _colour = colour;
		_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
		_coord = coord;
		_direct = direct;
	}
	bool MakeMove(Coord coord = Coord()) override{
		if (_lastDamage < 1)
			_colour = (int)(_lastDamage * 10) % 2 ? Colour(255, 255, 255) : _colorStabile;
		else
			_colour = _colorStabile;
		Rotate();
		Moving(coord);
		if (TouchedFrame()) {
			_direct.x *= TouchedFrameX() ? -1 : 1;
			_direct.y *= TouchedFrameY() ? -1 : 1;
		}
		return true;
	}
	void Rotate() override {
		_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
		for (int i = 0; i < _dots.size(); i++)
		{
			float x = _dots[i].x * cos(_turnÑounter) + _dots[i].y * sin(_turnÑounter);
			float y = _dots[i].y * cos(_turnÑounter) - _dots[i].x * sin(_turnÑounter);
			_dots[i].x = x;
			_dots[i].y = y;
		}
		_turnÑounter += 3.1416 / 2400;
	}
};

class TrackingEnemy: public Enemy {
	float _dist;
	void Distortion() {
		_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
		Rotate(3.1416 / 4);
		for (int i = 0; i < _dots.size(); ++i) {
			_dots[i].x += (i%2?cos(_dist): sin(_dist))*signbit(_dots[i].x-_coord.x)*5;
			_dots[i].y += (i % 2 ? sin(_dist):cos(_dist)) * signbit(_dots[i].y - _coord.y)*5 ;
		}
		_dist += 3.1416 / 600;
	}
public:
	TrackingEnemy( Coord coord, Direct direct = Direct(0,1), int level = 0, Colour colour = Colour(255, 0, 255)) {
		_dist = 0;
		_life = 5;
		_speed = 0.25 + level * 0.025;
		_colorStabile = _colour = colour;
		_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
		Rotate(3.1416 / 4);
		_coord = coord;
	}
	bool MakeMove(Coord coord = Coord()) override {
		if (_lastDamage < 1)
			_colour = (int)(_lastDamage * 10) % 2 ? Colour(255, 255, 255) : _colorStabile;
		else
			_colour = _colorStabile;
		Distortion();
		Moving(coord);
		if (TouchedFrame()) {
			_direct.x *= TouchedFrameX() ? -1 : 1;
			_direct.y *= TouchedFrameY() ? -1 : 1;
		}
		return true;
	}
	void Moving(Coord coord) override {
		_direct = Direct(-_coord.x + coord.x, -_coord.y + coord.y);
		_coord.x += _direct.x * _speed;
		_coord.y += _direct.y * _speed;
	}
};