#include "Object.h"

Explosion::Explosion(Coord coord) : _coord(coord) {
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
bool Explosion::MakeMove() {
	for (int i = 0; i < _count; i++) {
		_dots[i].x += _direct[i].x / _speed[i];
		_dots[i].y += _direct[i].y / _speed[i];
	}
	return (++life < 900 && !TouchedFrame()) ? false : true;
}
bool Explosion::TouchedFrame() {
	return TouchedFrameX() || TouchedFrameY();
}
bool Explosion::TouchedFrameX() {
	for (int i = 0; i < _dots.size(); i++)
		if ((_dots[i].x + _speed[i]*_direct[i].x) <= 0 || (_dots[i].x + _speed[i] * _direct[i].x) >= SCREEN_WIDTH) return true;
	return false;
}
bool Explosion::TouchedFrameY() {
	for (int i = 0; i < _dots.size(); i++)
		if ((_dots[i].y + _speed[i] * _direct[i].y) <= 0 || (_dots[i].y + _speed[i] * _direct[i].y) >= SCREEN_HEIGHT) return true;
	return false;
}

void Object::Rotate() {
	for (int i = 0; i < _dots.size(); ++i) {
		_dots[i].x *= cos(_direct.x);
		_dots[i].y *= sin(_direct.y);
	}
}
void Object::Rotate(float fi) {
	for (int i = 0; i < _dots.size(); ++i) {
		float x = _dots[i].x * cos(fi) + _dots[i].y * sin(fi);
		float y = _dots[i].y * cos(fi) - _dots[i].x * sin(fi);
		_dots[i].x = x;
		_dots[i].y = y;
	}
}
Coord Object::getCoord() {
	return _coord;
}
Direct Object::getDirect() {
	return _direct;
}
bool Object::TouchedFrame() {
	return TouchedFrameX() || TouchedFrameY();
}
bool Object::TouchedFrameX() {
	for (int i = 0; i < _dots.size(); i++)
		if ((-_dots[i].x + _coord.x+_speedX*_direct.x) <= 0 || (-_dots[i].x + _coord.x + _speedX * _direct.x) >= SCREEN_WIDTH) return true;
	return false;
}
bool Object::TouchedFrameY() {
	for (int i = 0; i < _dots.size(); i++)
		if ((-_dots[i].y + _coord.y - _speedY * _direct.y) <= 0 || (-_dots[i].y + _coord.y - _speedY * _direct.y) >= SCREEN_HEIGHT) return true;
	return false;
}
bool Object::IsIncluded(Coord point) {
	std::vector<float> lambda(_dots.size());
	bool l = false;
	for (int t = 0; t < _dots.size() - 2; t++) {
		bool s = true;
		int d[] = { 0, 1 + t, 2 + t };
		for (int i = 0; i < 3; i++)
		{
			int o = (0 - 1) % 3;
			int k1 = d[i], k2 = d[(i + 1) % 3], k0 = d[(i - 1) % 3 >= 0 ? (i - 1) % 3 : (i - 1) % 3 + 3];
			lambda[i] = ((_dots[k1].x - _dots[k2].x) * (point.y - _dots[k2].y - _coord.y) - (_dots[k1].y - _dots[k2].y) * (point.x - _dots[k2].x - _coord.x)) / ((_dots[k1].x - _dots[k2].x) * (_dots[k0].y - _dots[k2].y) - (_dots[k1].y - _dots[k2].y) * (_dots[k0].x - _dots[k2].x));
			s = s && (lambda[i] >= 0);
		}
		l = l || s;
	}
	return l;
}
void Object::Moving(float& dt, Coord coord) {
	unsigned int start_time = clock();
	_coord.x += _direct.x * _speedX * 60 * dt;
	_coord.y -= _direct.y * _speedY * 60 * dt;
	dt += (clock() - start_time) / 1000.0;
}

Weapon::Weapon(Coord coord, Direct direct, int level, Colour colour) {
	_colour = colour;
	_dots = Shapes::weapon(SCREEN_HEIGHT / 100);
	_coord = coord;
	_direct = direct;
	_speedX = _speedY = (float)16 + 1 * level;
	//PlaySound("shut.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC );
}
void Weapon::MakeMove(float& dt) {
	Moving(dt);
}

Ship::Ship(Coord coord , Direct direct , Colour colour ) {
	float a = SCREEN_HEIGHT / 20;
	_colour = colour;
	_dots = Shapes::ship();
	for (int i = 0; i < _dots.size(); _dots[i++] *= a);
	_coord = coord;
	_speedX = _speedY = 0;
	_direct = direct;
	D3 = false;
}
void Ship::ChangeDirection(int x, int y) {
	_direct = Direct(x - _coord.x, y - _coord.y);
	Rotate();
}
void Ship::Moving(float& dt, Coord c )  {
	unsigned int start_time = clock();
	_coord.x += _speedX * dt * 60;
	_coord.y += _speedY * dt * 60;
	dt += (clock() - start_time) / 1000.0;
	if (TouchedFrame()) {
		int s = _speedX / abs(_speedX);
		if (TouchedFrameX()) {
			_speedX *= -1;
			for (int i = 0; i < _dots.size(); i++) {
				if ((-_dots[i].x + _coord.x) <= 0)
					_coord.x += _dots[i].x - _coord.x+5;
				if( (-_dots[i].x + _coord.x) >= SCREEN_WIDTH)
					_coord.x -= -SCREEN_WIDTH -_dots[i].x + _coord.x+5;
			}
		}
		if (TouchedFrameY()) {
			_speedY *= -1;
			for (int i = 0; i < _dots.size(); i++) {
				if ((-_dots[i].y + _coord.y) <= 0)
					_coord.y += _dots[i].y - _coord.y+5;
				if ((-_dots[i].y + _coord.y) >= SCREEN_HEIGHT)
					_coord.y -= -SCREEN_HEIGHT - _dots[i].y + _coord.y+5;
			}
		}
		//_speedX *= TouchedFrameX() ? -1 * s : 1 * s;
		//_speedY *= TouchedFrameY() ? -1 * s : 1 * s;

	}
}
void Ship::MakeMove(float& dt) {
	_speedX += _boost * _direct.x * dt * 60;
	_speedY += _boost * _direct.y * dt * 60;
	Moving(dt);
}
void Ship::Rotate()  {
	_dots = Shapes::ship(SCREEN_HEIGHT / 20);
	if (D3)
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
void Ship::Braking(float& dt) {
	unsigned int start_time = clock();
	_speedX += _boost / 2 * (_speedX > 0 ? -1 : 1) * dt * 60;
	_speedY += _boost / 2 * (_speedY > 0 ? -1 : 1) * dt * 60;
	dt += (clock() - start_time) / 1000.0;
	Moving(dt);
}
Coord Ship::getShipNose() {
	return _dots[0];
}
void Ship::Roll() {
	Direct roll = _direct + Direct(_speedX, _speedY);
	float t = -atan2(roll.y, roll.x) - 3.1416 / 2;
	for (int i = 1; i < _dots.size(); i += 2)
	{
		float x = _dots[i].x * cos(t) + _dots[i].y * sin(t);
		float y = _dots[i].y * cos(t) - _dots[i].x * sin(t);
		_dots[i].x = x;
		_dots[i].y = y;
	}
}
void Ship::Deth() {
	_coord = Coord(-100, -100);
}
void Ship::set3D() {
	D3 = !D3;
}

bool Enemy::ChackDamage(Coord point, float db) {
	if (IsIncluded(point)) {
		_lastDamage *= _lastDamage > 0.25 ? 0 : 1;
		_life -= (_lastDamage == 0);
	}
	_lastDamage += db;

	return _life == 0;
}

StableEnemy::StableEnemy(Coord coord, Direct direct, int level, Colour colour) {
	_life = 3;
	_speedX =_speedY= 2 + level * 0.25;
	_colorStabile = _colour = colour;
	_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
	_coord = coord;
	_direct = direct;
}
bool StableEnemy::MakeMove(float& dt, Coord coord) {
	unsigned int start_time = clock();
	if (_lastDamage < 1)
		_colour = (int)(_lastDamage * 10) % 2 ? Colour(255, 255, 255) : _colorStabile;
	else
		_colour = _colorStabile;
	Rotate();
	dt += (clock() - start_time) / 1000.0;
	Moving(dt, coord);
	start_time = clock();
	if (TouchedFrame()) {
		_direct.x *= TouchedFrameX() ? -1 : 1;
		_direct.y *= TouchedFrameY() ? -1 : 1;
	}
	dt += (clock() - start_time) / 1000.0;
	return true;
}
void StableEnemy::Rotate(){
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

void TrackingEnemy::Distortion() {
	_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
	Rotate(3.1416 / 4);
	for (int i = 0; i < _dots.size(); ++i) {
		_dots[i].x += (i % 2 ? cos(_dist) : sin(_dist)) * signbit(_dots[i].x - _coord.x) * 5;
		_dots[i].y += (i % 2 ? sin(_dist) : cos(_dist)) * signbit(_dots[i].y - _coord.y) * 5;
	}
	_dist += 3.1416 / 600;
}
TrackingEnemy::TrackingEnemy(Coord coord, Direct direct , int level, Colour colour) {
	_dist = 0;
	_life = 3;
	_speedX=_speedY = 1 + level * 0.25;
	_colorStabile = _colour = colour;
	_dots = Shapes::enemy(SCREEN_HEIGHT / 20);
	Rotate(3.1416 / 4);
	_coord = coord;
}
bool TrackingEnemy::MakeMove(float& dt, Coord coord) {
	unsigned int start_time = clock();
	if (_lastDamage < 1)
		_colour = (int)(_lastDamage * 10) % 2 ? Colour(255, 255, 255) : _colorStabile;
	else
		_colour = _colorStabile;
	Distortion();
	//_direct = Direct(-_coord.x + coord.x, -_coord.y + coord.y);
	dt += (clock() - start_time) / 1000.0;
	Moving(dt, coord);
	start_time = clock();
	if (TouchedFrame()) {
		_direct.x *= TouchedFrameX() ? -1 : 1;
		_direct.y *= TouchedFrameY() ? -1 : 1;
	}
	dt += (clock() - start_time) / 1000.0;
	return true;
}
void TrackingEnemy::Moving(float dt, Coord coord) {
	_direct = Direct(-_coord.x + coord.x, -_coord.y + coord.y);
	_coord.x += _direct.x * _speedX * 60 * dt;
	_coord.y += _direct.y * _speedY * 60 * dt;
}


extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Explosion& e) {
	for (int i = 0; i < e._count; ++i) {
		if (!e.TouchedFrame())
			try {
			if ((e._dots[i].y != e._dots[i].y) || (e._dots[i].x != e._dots[i].x)&& !(e._dots[i].x < 0 || e._dots[i].x >= SCREEN_WIDTH || e._dots[i].y < 0 || e._dots[i].y >= SCREEN_HEIGHT))
				throw 1;
			image[(int)(e._dots[i].y)][(int)(e._dots[i].x)] = e._colour[i];
		}
		catch (...) {

		}
	}
}
extern void Draw(uint32_t(*image)[SCREEN_WIDTH], Object& o, unsigned int** img) {
	if (!img)
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
				if ((steep&&!(x < 0 || x >= SCREEN_HEIGHT || y < 0 || y >= SCREEN_WIDTH))||(!steep && !(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)))
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
		std::vector<Coord> texture = { Coord(21, 0), Coord(44,33), Coord(21, 33) , Coord(0,33)};
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
						if (!(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)) {
							int t1 = texture[d[2]].y * lambda[0] + texture[d[0]].y * lambda[1] + texture[d[1]].y * lambda[2];
							int t2 = texture[d[2]].x * lambda[0] + texture[d[0]].x * lambda[1] + texture[d[1]].x * lambda[2];
							if (t1 < 33, t2 < 44)
								buffer[y][x] = img[t1][t2];//Colour(255, 255, 255);
						}
				}
		}
	}
}