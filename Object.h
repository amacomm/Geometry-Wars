#pragma once
#include "Player.h"
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
	Explosion(Coord coord = Coord());
	bool MakeMove();
	bool TouchedFrame();
	bool TouchedFrameX();
	bool TouchedFrameY();
};

class Object {
protected:
	float _speed;
	std::vector<Coord> _dots;
	Coord _coord;
	Direct _direct;
	Colour _colour;
	virtual void Moving(float& dt, Coord coord = Coord());

public:
	friend void Draw(uint32_t(*image)[SCREEN_WIDTH], Object&, unsigned int**);
	virtual void Rotate();
	virtual void Rotate(float fi);
	Coord getCoord();
	Direct getDirect();
	bool TouchedFrame();
	bool TouchedFrameX();
	bool TouchedFrameY();
	bool IsIncluded(Coord point);
};

class Weapon : public Object {
public:
	Weapon(Coord coord, Direct direct, int level = 0, Colour colour = Colour(255, 255, 0));
	void MakeMove(float& dt);
};

class Ship final: public Object {
	float _speed2 = 0;
	float _boost = (float)1 / 30;
	bool D3;
public:
	Ship(Coord coord = Coord(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), Direct direct = Direct(0, 1), Colour colour = Colour(255, 255, 255));
	void ChangeDirection(int x, int y);
	void Moving(float& dt, Coord c = Coord()) override;
	void MakeMove(float& dt);
	void Rotate() override;
	void Braking(float& dt);
	Coord getShipNose();
	void Roll();
	void Deth();
	void set3D();
};

class Enemy: public Object {
protected:
	char _life;
	float _turn—ounter = 0;
	float _lastDamage=1;
	Colour _colorStabile;
public:
	bool ChackDamage(Coord point, float db);
	virtual bool MakeMove(float &dt, Coord coord = Coord()) = 0;
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
	StableEnemy(Coord coord, Direct direct, int level = 0, Colour colour = Colour(255, 0, 0));
	bool MakeMove(float& dt, Coord coord = Coord()) override;
	void Rotate() override;
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
	TrackingEnemy(Coord coord, Direct direct = Direct(0, 1), int level = 0, Colour colour = Colour(255, 0, 255));
	bool MakeMove(float& dt, Coord coord = Coord()) override;

	void Moving(float dt, Coord coord);
};

struct Script
{
	static std::vector<Enemy*> one(int level = 0) {
		int e2 = (rand() % (level < 1 ? 1 : level)) % 11;
		int e1 = 10 - e2;
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		bool line = rand() % 2, column = rand() % 2;
		for (int i = 0; i < n; ++i) {
			if (line == column) {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord((100 + i * 82)% SCREEN_WIDTH, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1 + (1 - line) * 2), level));
				else
					vec.push_back(new StableEnemy(Coord(100 + i * 82, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1 + (1 - line) * 2), level));
			}
			else {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, (100 + i * 57) % SCREEN_HEIGHT), Direct(-1 + column * 2, 0), level));
				else
					vec.push_back(new StableEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, 100 + i * 57), Direct(-1 + column * 2, 0), level));
			}
		}
		return vec;
	}

	static std::vector<Enemy*> two(int level = 0) {
		int e2 = (rand() % (level < 1 ? 1 : level)) % 11;
		int e1 = 10 - e2;
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		for (int i = 0; i < n; ++i) {
			if (i % 2 && e2-- > 0)
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH / 2 - 5 + i, SCREEN_HEIGHT / 2 - 5 + i), Direct(3.1416 / 2 - 3.1416 / 5 * i), level));
			else
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH / 2 - 5 + i, SCREEN_HEIGHT / 2 - 5 + i), Direct(3.1416 / 2 - 3.1416 / 5 * i), level));
		}
		return vec;
	}

	static std::vector<Enemy*> three(int level = 0) {
		int e2 = (rand() % (level < 1 ? 1 : level)) % 11;
		int e1 = 10 - e2;
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		for (int i = 0; i < 3; ++i) {
			if (i % 2 && e2-- > 0) {
				e2 -= 3;
				vec.push_back(new TrackingEnemy(Coord(100, 100), Direct(-3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH - 100, 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new TrackingEnemy(Coord(100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
			}
			else
			{
				vec.push_back(new StableEnemy(Coord(100, 100), Direct(-3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH - 100, 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
				vec.push_back(new StableEnemy(Coord(100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i), level));
			}
		}
		return vec;
	}
};
extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Explosion& e);
extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Object& o, unsigned int** img = nullptr);