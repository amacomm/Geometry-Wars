#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include "Object.h"
#pragma comment(lib, "Winmm.lib")


//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()

Ship ship;
std::vector<Enemy*> enemyBuffer;
std::vector<Enemy*> enemy;
std::vector<Weapon> weapon;
std::vector<Explosion> explosion;
std::vector<Create> create;
std::vector<unsigned int**> alf;

float const pauseConst = 2;
float const enemyCreateConst = 0.25;
float const shootConst = 0.2;

float pause;
float enemyCreate;
float shootTime;
Player player;
float start;
bool gameover;

void restart() {
    finalize();
    player.Deth();
    initialize();
}

unsigned int** image;
unsigned int** imShip;
unsigned int **medusa;
// initialize game data in this function
void initialize()
{
    srand(time(0));
    start = 0.001;
    image = new unsigned int* [SCREEN_HEIGHT+10];
    for (int i = 0; i < SCREEN_HEIGHT+10; i++)
        image[i] = new unsigned int[SCREEN_WIDTH+10];
    loader(image, (char*)"picture/amacomm1.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	imShip = new unsigned int* [34];
	for (int i = 0; i < 34; i++)
		imShip[i] = new unsigned int[45];
	loader(imShip, (char*)"picture/ship.bmp", 45, 34);
	//medusa = new unsigned int* [50];
	//for (int i = 0; i < 50; i++)
	//	medusa[i] = new unsigned int[50];
	//loader(medusa, (char*)"picture/medusa.bmp", 50, 50);
    alf = ABCc();
    PlaySound("music/amacomm.wav", NULL, SND_ASYNC );
    
    /*for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            image[i][j] = rand() % 1000 == 0 ? Colour(255, 255, 255) : Colour(0, 0, 0);
        }
    }*/
    pause = pauseConst;
    enemyCreate = enemyCreateConst;
    shootTime = 0;
    player = Player();
    gameover = false;
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
	unsigned int start_time = clock();
	if (dt < 1.0e-10)
		dt = 1.0e-5;
	if (start < 3) {
		start += dt;
		start = start >= 3 ? 10 : start;
		return;
	}
	else if (start == 10) {
		PlaySound("music/music_wave.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
		ship = Ship();
		enemy = Script::one();
		for (int i = 0; i < SCREEN_HEIGHT + 10; i++) {
			for (int j = 0; j < SCREEN_WIDTH + 10; j++) {
				image[i][j] = rand() % 1000 == 0 ? Colour(255, 255, 255) : Colour(0, 0, 0);
			}
		}
		start -= 1;
	}

	if (enemy.size() == 0 && enemyBuffer.size() == 0) {
		//enemy.push_back(new TrackingEnemy(Coord(100, 100)));

		pause -= dt;
		if (pause <= 0) {
			pause = pauseConst;
			int r = rand() % 3;
			enemyBuffer = r == 0 ? Script::one(player.getLevel()) : r == 1 ? Script::two(player.getLevel()) : Script::three(player.getLevel());
			enemy.push_back(enemyBuffer[0]);
			enemyBuffer.erase(enemyBuffer.begin());
		}
	}
	if (enemyBuffer.size() > 0) {
		enemyCreate -= dt;
		if (enemyCreate <= 0) {
			enemyCreate = enemyCreateConst;
			enemy.push_back(enemyBuffer[0]);
			enemyBuffer.erase(enemyBuffer.begin());
		}
	}

	if (is_mouse_button_pressed(0) && shootTime <= 0) {
		shootTime = shootConst - (0.015 * player.getLevel() > 0.15 ? 0.15 : 0.015 * player.getLevel());
		weapon.push_back(Weapon(ship.getCoord() - ship.getShipNose(), Direct(ship.getDirect().x - ship.getShipNose().x, ship.getDirect().y + ship.getShipNose().y), player.getLevel()));
	}
	shootTime -= dt;
	ship.ChangeDirection(get_cursor_x(), get_cursor_y());
	start_time = clock()-start_time ;
	dt += start_time/1000.0;
	start_time = clock();
	if (is_key_pressed('W'))
		ship.MakeMove(dt);
	else if (is_key_pressed('S'))
		ship.Braking(dt);
	else ship.Moving(dt);
	if (is_key_pressed('X') && shootTime <= 0)
		ship.set3D();
	start_time = clock()-start_time ;
	dt += start_time/1000.0;
	start_time = clock();
	for (int i = 0; i < explosion.size(); i++)
		if (explosion[i].MakeMove()) explosion.erase(explosion.begin() + i--);
	for (int i = 0; i < create.size(); i++)
		create[i].MakeMove();
	start_time = clock()-start_time ;
	dt += start_time/1000.0;
	start_time = clock();
	for (int i = 0; i < weapon.size(); i++) {
		weapon[i].MakeMove(dt);
		start_time = clock()-start_time ;
		dt += start_time/1000.0;
		start_time = clock();
		if (weapon[i].TouchedFrame()) {
			weapon.erase(weapon.begin() + i--);
			continue;
		}
		for (int j = 0; j < enemy.size(); j++) {
			if (enemy[j]->ChackDamage(weapon[i].getCoord(), dt)) {
				player.addScore();
				explosion.push_back(Explosion(enemy[j]->getCoord()));
				delete enemy[j];
				enemy.erase(enemy.begin() + j--);
				continue;
			}
			if (enemy[j]->IsIncluded(weapon[i].getCoord())) {
				weapon.erase(weapon.begin() + i--);
				break;
			}
		}
	}
	for (int i = 0; i < enemy.size(); i++) {
		enemy[i]->MakeMove(dt, ship.getCoord());
		start_time = clock()-start_time ;
		dt += start_time/1000.0;
		start_time = clock();
		if (enemy[i]->IsIncluded(ship.getCoord())) {
			explosion.push_back(Explosion(ship.getCoord()));
			ship.Deth();
			gameover = true;
			break;
		}
	}
	if (gameover) {
		if (is_key_pressed(VK_SPACE))
			restart();
		return;
	}

	if (is_key_pressed(VK_ESCAPE))
		schedule_quit_game();
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
	// clear backbuffer
	//memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    Direct d = Direct(-ship.getCoord().x + SCREEN_WIDTH/2, -ship.getCoord().y + SCREEN_HEIGHT / 2);
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			buffer[i][j] = image[i + (int)(d.y * 5) + 5][j + (int)(d.x * 5) + 5];
		}
	}
	Draw(buffer, ship, imShip);
	for (int i = 0; i < enemy.size(); i++)
		Draw(buffer, *enemy[i]/*, medusa*/);
	for (int i = 0; i < weapon.size(); i++)
		Draw(buffer, weapon[i]);
	for (int i = 0; i < explosion.size(); i++)
		Draw(buffer, explosion[i]);
    Draw(buffer, player, alf);
    if (gameover)
        Draw(buffer, "press space", alf);
}

// free game data in this function
void finalize()
{
    for (int i = 0; i < alf.size(); i++)
    {
        for (int j = 0; j < 20; j++)
            delete[] alf[i][j];
        delete[] alf[i];
    }
    for (int i = 0; i < enemyBuffer.size(); i++)
        delete enemyBuffer[i];
    enemyBuffer.clear();
    for (int i = 0; i < enemy.size(); i++)
        delete enemy[i];
    enemy.clear();
    weapon.clear();
    explosion.clear();
    for (int i = 0; i < SCREEN_HEIGHT; i++)
        delete[] image[i];
    delete[] image;
	for (int i = 0; i < 34; i++)
		delete[] imShip[i];
	delete[] imShip;
}

