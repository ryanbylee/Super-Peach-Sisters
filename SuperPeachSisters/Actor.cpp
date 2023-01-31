#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//---------------------------
//ACTOR
Actor::Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW, bool isAlive)
	: GraphObject(imageID, startX, startY, dir, depth, size), m_isAlive(true), m_isDamagable(false), m_isPassable(true), m_StudentWorld(SW)
{
	

}
bool Actor::isPassable() {
	return m_isPassable;

}

bool Actor::isDamagable() {
	return m_isDamagable;
}

bool Actor::isAlive() {
	return m_isAlive;
}

void Actor::setAlive(bool val) {
	m_isAlive = val;
}

StudentWorld* Actor::getWorld() {
	return m_StudentWorld;
}

void Actor::damage() {
	if (isDamagable()) {
		getWorld()->increaseScore(100);
		setAlive(false);
	}
	
}
//---------------------------
//GROUND: base for Block and Pipe

Ground::Ground(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW, int goodies)
	: Actor(imageID, startX, startY, dir, depth, size, SW, true), m_goodie(goodies), m_releasedGoodie(false)
{
}

void Ground::doSomething() {
	return;
}

void Ground::bonk() {
	if (m_goodie <= 0 || m_releasedGoodie) {
		getWorld()->playSound(SOUND_PLAYER_BONK);
	}
	if (m_goodie > 0 && !m_releasedGoodie) {
		m_releasedGoodie = true;
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		if (m_goodie == 1) {
			getWorld()->addActor(new Flower(getWorld(), getX(), getY() + 8));
		}
		if (m_goodie == 2) {
			getWorld()->addActor(new Star(getWorld(), getX(), getY() + 8));
		}
		if (m_goodie == 3) {
			getWorld()->addActor(new Mushroom(getWorld(), getX(), getY() + 8));
		}

	}
}

bool Ground::isPassable() {
	return false;
}

bool Ground::isDamagable() {
	return false;
}
//---------------------------
//BLOCK

Block::Block(StudentWorld* SW, int startX, int startY, int goodies)
	: Ground(IID_BLOCK, startX, startY, 0, 2, 1.0, SW, goodies)
{	
}

//----------------------------
//PIPE

Pipe::Pipe(StudentWorld* SW, int startX, int startY) 
	: Ground(IID_PIPE, startX, startY, 0, 2, 1.0, SW, 0) 
{
}


//--------------------------------
//PEACH

Peach::Peach(StudentWorld* SW, int startX, int startY)
	: Actor(IID_PEACH, startX, startY, 0, 0, 1.0, SW, true), m_health(1), m_remaining_jump_distance(0), m_jumpPower(false), m_shootPower(false), m_starPower(false), m_timeRechargeFire(0), m_starDuration(0), m_hasTempInvinc(false), m_tempInvinc(0)
{

}

bool Peach::isDamagable() {
	return true;
}

void Peach::damage() {
	bonk();
}

bool Peach::hasJump() {
	return m_jumpPower;
}
bool Peach::hasShoot() {
	return m_shootPower;
}
bool Peach::hasStar() {
	return m_starPower;
}

void Peach::setJump(bool val) {
	m_jumpPower = val;
}
void Peach::setShoot(bool val) {
	m_shootPower = val;
}
void Peach::setStar(bool val) {
	m_starPower = val;
}

int Peach::starDuration() {
	return m_starDuration;
}

void Peach::incStarDuration(int amt) {
	m_starDuration += amt;
}

bool Peach::hasTempInvinc() {
	return m_hasTempInvinc;
}

int Peach::getTempInvic() {
	return m_tempInvinc;
}

void Peach::addTempInvinc(int amt) {
	m_tempInvinc += amt;
}
	
void Peach::doSomething() {
	
	if (!isAlive()) {
		return;
	}
	if (hasStar()) {
		if (starDuration() > 0) {
			incStarDuration(-1);
		}
		else {
			setStar(false);
		}
	}
	if (hasTempInvinc()) {
		if (getTempInvic() > 0) {
			addTempInvinc(-1);
		}
		else {
			m_hasTempInvinc = false;
		}
	}
	if (m_remaining_jump_distance > 0) {
		int targetY = getY() + 4;
		Actor* temp = nullptr;
		if (!getWorld()->isHitboxAt(getX() + (SPRITE_WIDTH / 2), targetY + (SPRITE_HEIGHT / 2), temp)) {
			moveTo(getX(), targetY);
			m_remaining_jump_distance--;
		}
		else if (temp->isPassable()) {
			moveTo(getX(), targetY);
			m_remaining_jump_distance--;
			temp->bonk();
		}
		else 
		{
			if (temp != nullptr) {
				temp->bonk();
				m_remaining_jump_distance = 0;
			}
			
		}
	}
	else {
		bool block = false;
		Actor* below = nullptr;
		for (int i = 1; i <= 3; i++) {
			if (getWorld()->isHitboxAt(getX() + (SPRITE_WIDTH / 2), getY() - i, below) && !below->isPassable()) {
				block = true;
			}
			else if (below != nullptr && below->isPassable()) {
				below->bonk();
			}
		}
		if (!block) {
			moveTo(getX(), getY() - 4);
		}
		
	}
	
	if (m_timeRechargeFire > 0) {
		m_timeRechargeFire--;
	}

	int ch;

	if (getWorld()->getKey(ch)) {
		int targetX;
		int targetY;
		Actor* temp = nullptr;
		switch (ch) {
			case KEY_PRESS_LEFT:
				setDirection(180);
				targetX = getX() - 4;
				if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH / 2, getY() + SPRITE_HEIGHT / 2, temp)) {
					moveTo(targetX, getY());	
				}
				else if (temp->isPassable()) {
					moveTo(targetX, getY());
					temp->bonk();
				}
				else {
					if (temp != nullptr) {
						temp->bonk();
					}
				//	getWorld()->actorAt(targetX - SPRITE_WIDTH / 2, getY(), temp);
					
				}
				break;
			case KEY_PRESS_RIGHT:
				setDirection(0);
				targetX = getX() + 4;
				if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH / 2, getY() + SPRITE_HEIGHT / 2, temp)) {
					moveTo(targetX, getY());
				}
				else if (temp->isPassable()) {
					moveTo(targetX, getY());
					temp->bonk();
				}
				else {
					if (temp != nullptr) {
						temp->bonk();
					}
					//	getWorld()->actorAt(targetX - SPRITE_WIDTH / 2, getY(), temp);

				}
				break;
			case KEY_PRESS_UP:
				
				if (getWorld()->isHitboxAt(getX() + SPRITE_WIDTH / 2, getY() - 1, temp) && !temp->isPassable()) {
					if (!hasJump()) {
						m_remaining_jump_distance = 8;
					}
					else {
						m_remaining_jump_distance = 12;
					}
					getWorld()->playSound(SOUND_PLAYER_JUMP);
					
				}
				break;
			case KEY_PRESS_SPACE:
				if (hasShoot()) {
					if (m_timeRechargeFire == 0) {
						getWorld()->playSound(SOUND_PLAYER_FIRE);
						m_timeRechargeFire = 8;
						int targetX;
						if (getDirection() == 0) {
							targetX = getX() + 4;
							getWorld()->addActor(new PeachFire(getWorld(), targetX, getY(), 0));
						}
						else if (getDirection() == 180) {
							targetX = getX() - 4;
							getWorld()->addActor(new PeachFire(getWorld(), targetX, getY(), 180));
						}
					}
				}
			
			}
		}
	
}

void Peach::bonk() {
	if (hasStar() || hasTempInvinc()) { // dummy: temporary invincible to be added
		return;
	}
	else {
		decreaseHealth(1);
		//set temp invincible to 10
		addTempInvinc(10);
		m_hasTempInvinc = true;
		if (hasShoot()) {
			setShoot(false);
		}
		if (hasJump()) {
			setJump(false);
		}
		if (getHealth() > 0) {
			getWorld()->playSound(SOUND_PLAYER_HURT);
		}
		else {
			setAlive(false);
		}
	}
}

void Peach::setHealth(int amt) {
	m_health = amt;
}

void Peach::decreaseHealth(int amt) {
	m_health -= amt;
}

int Peach::getHealth() {
	return m_health;
}
//------------------------------------------------
//base for flag and mario
//GOAL


Goal::Goal(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW)
	: Actor(imageID, startX, startY, dir, depth, size, SW, true)
{
}

void Goal::bonk() {
	return;
}

void Goal::doSomething() {
	if (!isAlive()) {
		return;
	}
	else {
		if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
			getWorld()->increaseScore(1000);
			setAlive(false);
			getWorld()->setEnded(true);

		}
	}
}

//--------------------------------------
//FLAG

Flag::Flag(StudentWorld* SW, int startX, int startY) 
	: Goal(IID_FLAG, startX, startY, 0, 1, 1, SW) 
{
}

//----------------------------------------
//MARIO

Mario::Mario(StudentWorld* SW, int startX, int startY)
	: Goal(IID_MARIO, startX, startY, 0, 1, 1, SW)
{
}

void Mario::doSomething() {
	Goal::doSomething();
	getWorld()->setWon(true);
}

//------------------------------------------
//base for flower, star, mushroom
//GOODIES

Goodies::Goodies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW):
	Actor(imageID, startX, startY, dir, depth, size, SW, true) 
{
}

void Goodies::doSomething() {

	Actor* down = nullptr;
	if (!getWorld()->isHitboxAt(getX() + (SPRITE_WIDTH / 2), getY() - 1, down)) {
		moveTo(getX(), getY() - 2);
	}
	else if (down->isPassable()) {
		moveTo(getX(), getY() - 2);
	}
	int targetX;
	Actor* side = nullptr;

	if (getDirection() == 0) {
		targetX = getX() + 2;
		if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (3.0/4.0), getY() + SPRITE_HEIGHT / 2, side)) {
			moveTo(targetX, getY());
		}
		else if (side->isPassable()) {
			moveTo(targetX, getY());
		}
		else {
			setDirection(180);
			return;
			
		}
	}
	else if (getDirection() == 180) {
		targetX = getX() - 2;
		if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (1.0 / 4.0), getY() + SPRITE_HEIGHT / 2, side)) {
			moveTo(targetX, getY());
		}
		else if (side->isPassable()) {
			moveTo(targetX, getY());
		}
		else {
			setDirection(0);
			return;

		}
	}
}

void Goodies::bonk() {
	return;
}

void Goodies::informTheWorld(int score, int health) {
	getWorld()->increaseScore(score);
	getWorld()->getPlayer()->setHealth(health);
	setAlive(false);
	getWorld()->playSound(SOUND_PLAYER_POWERUP);
}
//-------------------------------------------
//FLOWER

Flower::Flower(StudentWorld* SW, int startX, int startY) 
:	Goodies(IID_FLOWER, startX, startY, 0, 1, 1, SW) 
{
}



void Flower::doSomething() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		Goodies::informTheWorld(50, 2);
		getWorld()->getPlayer()->setShoot(true);
		return;
	}
	else {
		Goodies::doSomething();
		return;
	}
}

Mushroom::Mushroom(StudentWorld* SW, int startX, int startY):
	Goodies(IID_MUSHROOM, startX, startY, 0, 1, 1, SW) {

}

void Mushroom::doSomething() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		Goodies::informTheWorld(75, 2);
		getWorld()->getPlayer()->setJump(true);
		return;
	}
	else {
		//dummy: has to move around
		Goodies::doSomething();
		return;
	}
}



Star::Star(StudentWorld* SW, int startX, int startY)
	: Goodies(IID_STAR, startX, startY, 0, 1, 1, SW)
{
}



void Star::doSomething() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		getWorld()->increaseScore(100);
		getWorld()->getPlayer()->setStar(true);
		getWorld()->getPlayer()->incStarDuration(150);
		setAlive(false);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		
		return;
	}
	else {
		//dummy: has to movearound
		Goodies::doSomething();
		return;
	}
}

//-------------------------------------------------
//PROJECTILE

Projectile::Projectile(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW):
	Actor(imageID, startX, startY, dir, depth, size, SW, true) 
{
}

void Projectile::doSomething() {
	projectileOverlap();
	Actor* down = nullptr;
	if (!getWorld()->isHitboxAt(getX() + (SPRITE_WIDTH / 2), getY() - 1, down)) {
		moveTo(getX(), getY() - 2);
	}
	else if (down->isPassable()) {
		moveTo(getX(), getY() - 2);
	}
	int targetX;
	Actor* side = nullptr;
	if (getDirection() == 0) {
		targetX = getX() + 2;
		if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (3.0 / 4.0) + 1, getY() + SPRITE_HEIGHT / 2, side)) {
			moveTo(targetX, getY());
		}
		else if (side->isPassable()) {
			moveTo(targetX, getY());
		}

		else {
			if (side != nullptr) {
				setAlive(false);
				return;
			}
			

		}
	}
	else if (getDirection() == 180) {
		targetX = getX() - 2;
		if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (1.0 / 4.0) - 1, getY() + SPRITE_HEIGHT / 2, side)) {
			moveTo(targetX, getY());
		}
		else if (side->isPassable()) {
			moveTo(targetX, getY());
		}
		else {
			if (side != nullptr) {
				setAlive(false);
				return;
			}

		}
	}

	
}

bool Projectile::isDamagable() {
	return false;
}

void Projectile::bonk() {
	return;
}

//-------------------------------------------
//PIRANHAFIRE

PiranhaFire::PiranhaFire(StudentWorld* SW, int startX, int startY, int dir) :
	Projectile(IID_PIRANHA_FIRE, startX, startY, dir, 1, 1, SW) {

}

void PiranhaFire::projectileOverlap() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		getWorld()->getPlayer()->damage();
		setAlive(false);
		return;
	}
}

//---------------------------------------------
//PEACHFIRE

PeachFire::PeachFire(StudentWorld* SW, int startX, int startY, int dir) :
	Projectile(IID_PEACH_FIRE, startX, startY, dir, 1, 1, SW) {

}

void PeachFire::projectileOverlap() {
	Actor* opp = nullptr;
	
	for (int i = 0; i < SPRITE_WIDTH; i++) {
		if (getWorld()->isHitboxAt(getX() + i, getY() + SPRITE_HEIGHT / 2, opp)) {
			if (opp != nullptr && opp != this && opp != getWorld()->getPlayer()) {
				if (opp->isDamagable()) {
					opp->damage();
					setAlive(false);
					return;
				}
				if (!opp->isPassable()) {
					setAlive(false);
					return;
				}
				
			}
			
		}
	}
	

	/*if (getWorld()->isHitboxAt(getX(), getY() + SPRITE_HEIGHT / 2, opp)) {
		if (opp != nullptr && opp != this && opp != getWorld()->getPlayer()) {
			if (opp->isDamagable()) {
				opp->damage();
				setAlive(false);
				return;
			}
			if (!opp->isPassable()) {
				setAlive(false);
				return;
			}

		}

	}
	*/
	
}

Shell::Shell(StudentWorld* SW, int startX, int startY, int dir):
	Projectile(IID_SHELL, startX, startY, dir, 1, 1, SW) 
{
}

void Shell::projectileOverlap() {
	Actor* opp = nullptr;
	for (int i = 0; i < SPRITE_WIDTH; i++) {
		if (getWorld()->isHitboxAt(getX() + i, getY() + SPRITE_HEIGHT / 2, opp)) {
			if (opp != nullptr && opp != this && opp != getWorld()->getPlayer() && opp->isAlive()) {
				if (opp->isDamagable()) {
					opp->damage();
					setAlive(false);
					return;
				}
				if (!opp->isPassable()) {
					setAlive(false);
					return;
				}

			}

		}
	}
}
//----------------------------------------
//Enemy

Enemy::Enemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW) :
	Actor(imageID, startX, startY, dir, depth, size, SW, true) 
{
}

void Enemy::informTheWorld() {
	getWorld()->playSound(SOUND_PLAYER_KICK);
	getWorld()->increaseScore(100);
	setAlive(false);
}

void Enemy::bonk() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		if (getWorld()->getPlayer()->hasStar()) {
			informTheWorld();
		}
	}
	return;
}

bool Enemy::isDamagable() {
	return true;
}

void Enemy::doSomething() {
	if (!isAlive()) {
		return;
	}
	else {
		if (getWorld()->overlap(this, getWorld()->getPlayer())) {
			getWorld()->getPlayer()->bonk();
			return;
		}



		int targetX;
		Actor* below = nullptr;
		Actor* side = nullptr;

		if (getDirection() == 0) {
			targetX = getX() + 1;
			if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (7.0 / 8.0) + 1, getY() + SPRITE_HEIGHT / 2, side)) {
				if (getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (7.0 / 8.0) + 1, getY() - 1, below)) {
					moveTo(targetX, getY());
				}
				else {
					setDirection(180);
				}

			}
			else if (side->isPassable()) {
				if (getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (7.0 / 8.0) + 1, getY() - 1, below)) {
					moveTo(targetX, getY());
				}
				else {
					setDirection(180);
				}
			}
			else {
				setDirection(180);
				return;

			}
		}
		else if (getDirection() == 180) {
			targetX = getX() - 1;
			if (!getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (1.0 / 8.0) - 1 , getY() + SPRITE_HEIGHT / 2, side)) {
				if (getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (1.0 / 8.0) - 1, getY() - 1, below)) {
					moveTo(targetX, getY());
				}
				else {
					setDirection(0);
				}

			}
			else if (side->isPassable()) {
				if (getWorld()->isHitboxAt(targetX + SPRITE_WIDTH * (1.0 / 8.0) - 1, getY() - 1, below)) {
					moveTo(targetX, getY());
				}
				else {
					setDirection(0);
				}
			}
			else {
				setDirection(0);
				return;

			}
		}
	}
}

Piranha::Piranha(StudentWorld* SW, int startX, int startY) :
	Enemy(IID_PIRANHA, startX, startY, randInt(0, 1) * 180, 0, 1, SW), fireDelay(0) 
{
}

void Piranha::doSomething() {
	if (!isAlive()) {
		return;
	}
	increaseAnimationNumber();
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		getWorld()->getPlayer()->bonk();
		return;
	}
	
	if ((getWorld()->getPlayer()->getY() <= getY() + (1.5 * SPRITE_HEIGHT)) && (getWorld()->getPlayer()->getY() >= getY() - (1.5 * SPRITE_HEIGHT))) {
		if (getWorld()->getPlayer()->getX() > getX()) {
			setDirection(0);
		}
		else if (getWorld()->getPlayer()->getX() < getX()) {
			setDirection(180);
		}
	}
	else {
		return;
	}
	if (fireDelay > 0) {
		fireDelay--;
		return;
	}
	else {
		int distanceBetX = getX() - getWorld()->getPlayer()->getX();
		if (distanceBetX < 0) {
			distanceBetX *= -1;
		}
		if (distanceBetX < (8 * SPRITE_WIDTH)) {
			getWorld()->addActor(new PiranhaFire(getWorld(), getX(), getY(), getDirection()));
			getWorld()->playSound(SOUND_PIRANHA_FIRE);
			fireDelay = 40;
		}
	}
	
}

Goomba::Goomba(StudentWorld* SW, int startX, int startY) :
	Enemy(IID_GOOMBA, startX, startY, randInt(0, 1) * 180, 0, 1, SW)
{
}

Koopa::Koopa(StudentWorld* SW, int startX, int startY) : 
	Enemy(IID_KOOPA, startX, startY, randInt(0, 1) * 180, 0, 1, SW) 
{
}

void Koopa::bonk() {
	if (getWorld()->overlap(getWorld()->getPlayer(), this)) {
		if (getWorld()->getPlayer()->hasStar()) {
			Enemy::informTheWorld();
			getWorld()->addActor(new Shell(getWorld(), getX(), getY(), getDirection()));
		}
	}
	return;
}

void Koopa::damage() {
	if (isDamagable()) {
		getWorld()->increaseScore(100);
		setAlive(false);
		getWorld()->addActor(new Shell(getWorld(), getX(), getY(), getDirection()));
	}
}
