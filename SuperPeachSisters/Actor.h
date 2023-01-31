#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW, bool isAlive);
	void virtual doSomething() = 0;
	void virtual bonk() = 0;
	bool virtual isPassable();
	bool virtual isDamagable();
	bool isAlive();
	void setAlive(bool val);
	StudentWorld* getWorld();
	void virtual damage();

private:
	int m_isAlive;
	bool m_isDamagable;
	bool m_isPassable;
	StudentWorld* m_StudentWorld;

};
//---------------------------------------
//PEACH

class Peach : public Actor {
public:
	Peach(StudentWorld* SW, int startX, int startY);
	
	void virtual doSomething();

	void virtual bonk();

	bool virtual isDamagable();
	void virtual damage();

	void setHealth(int amt);
	void decreaseHealth(int amt);
	int getHealth();

	bool hasJump();
	bool hasShoot();

	bool hasStar();
	int starDuration();
	void incStarDuration(int amt);

	bool hasTempInvinc();
	int getTempInvic();
	void addTempInvinc(int amt);


	void setJump(bool val);
	void setShoot(bool val);
	void setStar(bool val);

private:
	int m_health;
	int m_remaining_jump_distance;
	bool m_jumpPower;
	bool m_shootPower;
	int m_timeRechargeFire;
	bool m_starPower;
	int m_starDuration;

	bool m_hasTempInvinc;
	int m_tempInvinc;

};

//----------------------------------------
//base class for Block, pipe
// GROUND

class Ground : public Actor {
public:
	Ground(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW, int goodies);
	void virtual doSomething();
	void virtual bonk();
	bool virtual isPassable();
	bool virtual isDamagable();

private:
	int m_goodie;
	bool m_releasedGoodie;
};
//-----------------------------------------
//BLOCK

class Block : public Ground {
public:
	Block(StudentWorld* SW, int startX, int startY, int goodies);
private:
	
};

//------------------------------------------
//PIPE

class Pipe : public Ground {
public:
	Pipe(StudentWorld* SW, int startX, int startY);
};


//-------------------------------------------
//base class for flag and mario
//GOAL

class Goal : public Actor {
public:
	Goal(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW);
	void virtual bonk();
	void virtual doSomething();

};

//--------------------------------------------
//FLAG

class Flag : public Goal {
public:
	Flag(StudentWorld* SW, int startX, int startY);
	
};

//---------------------------------------------
//MARIO

class Mario : public Goal {
public:
	Mario(StudentWorld* SW, int startX, int startY);
	void virtual doSomething();
};

//----------------------------------------------
//base for flower, star, mushroom
//GOODIES

class Goodies : public Actor {
public:
	Goodies(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW);
	void virtual doSomething();
	void bonk();
protected:
	void informTheWorld(int score, int health);
};

//-----------------------------------------------
//FLOWER

class Flower : public Goodies {
public:
	Flower(StudentWorld* SW, int startX, int startY);
	void virtual doSomething();
};

//------------------------------------------------
//MUSHROOM

class Mushroom : public Goodies {
public:
	Mushroom(StudentWorld* SW, int startX, int startY);
	void virtual doSomething();
};

//-------------------------------------------------
//STAR

class Star : public Goodies {
public:
	Star(StudentWorld* SW, int startX, int startY);
	void virtual doSomething();
};

//--------------------------------------------------
//Base for piranaFireball, peachFireball, shell
//PROJECTILE

class Projectile : public Actor {
public:
	Projectile(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW);
	void virtual doSomething();
	void virtual projectileOverlap() = 0;
	bool virtual isDamagable();
	void virtual bonk();
};

//----------------------------------------------
//PiranhaFire

class PiranhaFire : public Projectile {
public:
	PiranhaFire(StudentWorld* SW, int startX, int startY, int dir);
	void virtual projectileOverlap();
};

//-----------------------------------------------
//PeachFire

class PeachFire : public Projectile {
public:
	PeachFire(StudentWorld* SW, int startX, int startY, int dir);
	void virtual projectileOverlap();
};

//------------------------------------------------------------
//Shell

class Shell : public Projectile {
public:
	Shell(StudentWorld* SW, int startX, int startY, int dir);
	void virtual projectileOverlap();
};

//-------------------------------------------------------------
//base for Piranha, Goomba, Koopa
//ENEMY

class Enemy : public Actor {
public:
	Enemy(int imageID, int startX, int startY, int dir, int depth, double size, StudentWorld* SW);
	void virtual bonk();
	bool virtual isDamagable();
	void virtual doSomething();
protected:
	void informTheWorld();
	
};

//------------------------------------------------
//PIRANHA

class Piranha : public Enemy {
public:
	Piranha(StudentWorld* SW, int startX, int startY);
	void virtual doSomething();

private:
	int fireDelay;
};

//-------------------------------------------------
//GOOMBA
class Goomba : public Enemy {
public:
	Goomba(StudentWorld* SW, int startX, int startY);
};

class Koopa : public Enemy {
public:
	Koopa(StudentWorld* SW, int startX, int startY);
	void virtual bonk();
	void virtual damage();
};

//--------------------------------------------------
//KOOPA
#endif // ACTOR_H_