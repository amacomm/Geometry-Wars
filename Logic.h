#pragma once
#include "Object.h"

struct Script
{
	static std::vector<Enemy*> one(int e1 = 10, int e2 = 0) {
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		bool line = rand() % 2, column = rand() % 2;
		for (int i = 0; i < n; ++i) {
			if (line == column) {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord(100 + i * 82, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1+(1-line)*2)));
				else
					vec.push_back(new StableEnemy(Coord(100 + i * 82, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1 + (1-line) * 2)));
			}
			else {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, 100 + i * 57), Direct(-1 + column *2, 0)));
				else
					vec.push_back(new StableEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, 100 + i * 57), Direct(-1 + column * 2, 0)));
			}
		}
		return vec;
	}

	static std::vector<Enemy*> two(int e1 = 10, int e2 = 0) {
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		for (int i = 0; i < n; ++i) {
			if (i % 2 && e2-- > 0)
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH / 2 - 5 + i, SCREEN_HEIGHT / 2 - 5 + i), Direct(3.1416 / 2 - 3.1416 / 5 * i)));
			else
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH / 2 - 5 + i, SCREEN_HEIGHT / 2 - 5 + i), Direct(3.1416 / 2 - 3.1416 / 5 * i)));
		}
		return vec;
	}

	static std::vector<Enemy*> three(int e1 = 10, int e2 = 0) {
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		for (int i = 0; i < n; ++i) {
			if (i % 2 && e2-- > 0) {
				e2 -= 3;
				vec.push_back(new TrackingEnemy(Coord(100, 100), Direct(-3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH - 100, 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new TrackingEnemy(Coord(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new TrackingEnemy(Coord(100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
			}
			else
			{
				vec.push_back(new StableEnemy(Coord(100, 100), Direct(-3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH - 100, 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new StableEnemy(Coord(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
				vec.push_back(new StableEnemy(Coord(100, SCREEN_HEIGHT - 100), Direct(3.1416 / 4 - 3.1416 / 2 * i)));
			}
		}
		return vec;
	}
};

class Player {
	int _score;
	int _level;
public:
	Player() {
		_score = 0;
		_level = 0;
	}
	int getScore() {
		return _score;
	}
	int getLevel() {
		return _level;
	}
	void addScore(int score = 25) {
		_score += score;
		if (_score / 1000 - _level > 0)
			++_level;
	}
	void Deth() {
		_score = 0;
		_level = 0;
	}
};