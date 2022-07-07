#pragma once
#include "Engine.h"
#include <string>
#include <vector>

class Player {
	int _score;
	int _level;
public:
	Player();
	int getScore();
	int getLevel();
	void addScore(int score = 25);
	void Deth();
};

void Draw(uint32_t(*image)[SCREEN_WIDTH], Player& player, std::vector<unsigned int**> alf);
void Draw(uint32_t(*image)[SCREEN_WIDTH], std::string text, std::vector<unsigned int**> alf);