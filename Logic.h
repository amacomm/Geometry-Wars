#pragma once
#include "Object.h"
#include <string>

struct Script
{
	static std::vector<Enemy*> one(int e1 = 10, int e2 = 0, int level = 0) {
		int s = (int)((0.9) * level);
 		e2 = (rand() % (s<1?1:s))%11;
		e1 = 10 - e2;
		std::vector<Enemy*> vec;
		int n = e1 + e2;
		bool line = rand() % 2, column = rand() % 2;
		for (int i = 0; i < n; ++i) {
			if (line == column) {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord(100 + i * 82, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1+(1-line)*2), level));
				else
					vec.push_back(new StableEnemy(Coord(100 + i * 82, line ? 100 : SCREEN_HEIGHT - 100), Direct(0, -1 + (1-line) * 2), level));
			}
			else {
				if (i % 2 && e2-- > 0)
					vec.push_back(new TrackingEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, 100 + i * 57), Direct(-1 + column *2, 0), level));
				else
					vec.push_back(new StableEnemy(Coord(column ? 100 : SCREEN_WIDTH - 100, 100 + i * 57), Direct(-1 + column * 2, 0), level));
			}
		}
		return vec;
	}

	static std::vector<Enemy*> two(int e1 = 10, int e2 = 0, int level = 0) {
		int s = (int)((0.9) * level);
		e2 = (rand() % (s < 1 ? 1 : s)) % 11;
		e1 = 10 - e2;
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

	static std::vector<Enemy*> three(int e1 = 10, int e2 = 0, int level = 0) {
		int s = (int)((0.9) * level);
		e2 = (rand() % (s < 1 ? 1 : s)) % 11;
		e1 = 10 - e2;
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
		if (_score / 250 - _level > 0)
			++_level;
	}
	void Deth() {
		_score = 0;
		_level = 0;
	}
};

void Draw(uint32_t(*image)[SCREEN_WIDTH], Player& player, std::vector<unsigned int**> alf) {
	int score = player.getScore();
	for (int i = 6; i >= 0; --i) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 17; k++) {
				image[j + 10][k + i * 20+20] = alf[score%10+26][j][k]; // alf[i][j][k] = img[j + i / 10][k + i % 10];
			}
		}
		score /= 10;
	}
}
void Draw(uint32_t(*image)[SCREEN_WIDTH], std::string text, std::vector<unsigned int**> alf) {
	
	for (int i = text.size()-1; i >= 0; --i) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 17; k++) {
				if (text[i] == ' ')
					continue;
				image[j + 20][k + i * 20 + SCREEN_HEIGHT/2] = alf[text[i]-'a'][j][k]; // alf[i][j][k] = img[j + i / 10][k + i % 10];
			}
		}
	}
}