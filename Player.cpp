#include "Player.h"

Player::Player() {
	_score = 0;
	_level = 0;
}
int Player::getScore() {
	return _score;
}
int Player::getLevel() {
	return _level;
}
void Player::addScore(int score) {
	_score += score;
	if ((_score / 250 - _level) > 0)
		++_level;
}
void Player::Deth() {
	_score = 0;
	_level = 0;
}

void Draw(uint32_t(*image)[SCREEN_WIDTH], Player& player, std::vector<unsigned int**> alf) {
	int score = player.getScore();
	for (int i = 6; i >= 0; --i) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 17; k++) {
				image[j + 10][k + i * 20 + 20] = alf[score % 10 + 26][j][k];
			}
		}
		score /= 10;
	}
}
void Draw(uint32_t(*image)[SCREEN_WIDTH], std::string text, std::vector<unsigned int**> alf) {

	for (int i = text.size() - 1; i >= 0; --i) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 17; k++) {
				if (text[i] == ' ')
					continue;
				image[j + 20][k + i * 20 + SCREEN_HEIGHT / 2] = alf[text[i] - 'a'][j][k];
			}
		}
	}
}