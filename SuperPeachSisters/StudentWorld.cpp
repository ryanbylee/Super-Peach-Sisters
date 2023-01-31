#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), m_player(new Peach(this, 0, 0)), m_isEnded(false), m_hasWon(false)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
    
}

Peach* StudentWorld::getPlayer() {
    return m_player;
}

bool StudentWorld::isEnded() {
    return m_isEnded;
}
void StudentWorld::setEnded(bool val) {
    m_isEnded = val;
}

bool StudentWorld::hasWon() {
    return m_hasWon;
}

void StudentWorld::setWon(bool val) {
    m_hasWon = val;
}

void StudentWorld::addActor(Actor* a) {
    m_actors.push_back(a);
}

void StudentWorld::deleteActor(Actor* a) {
    for (int i = 0; i < m_actors.size(); ) {
        if (m_actors[i] == a) {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
        }
    }
}
int StudentWorld::init()
{
    if (m_player == nullptr) {
        m_player = new Peach(this, 0, 0);
    }
   
    setEnded(false);
    ostringstream levelName;
    Level lev(assetPath());
    levelName.fill('0');
    levelName << "level" << setw(2) << getLevel() << ".txt";

    string level_file = levelName.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success) {
        Level::GridEntry ge;
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                ge = lev.getContentsOf(i, j); // x=5, y=10
                switch (ge)
                {
                case Level::empty:
                    break;
                case Level::koopa:
                    m_actors.push_back(new Koopa(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::goomba:
                    m_actors.push_back(new Goomba(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::peach:
                    m_player->moveTo(i * SPRITE_WIDTH, j * SPRITE_HEIGHT);
                    break;
                case Level::flag:
                    m_actors.push_back(new Flag(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 0));
                    break;
                case Level::flower_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 1));
                    break;
                case Level::star_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 2));
                    break;
                case Level::mushroom_goodie_block:
                    m_actors.push_back(new Block(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT, 3));
                    break;
                case Level::pipe:
                    m_actors.push_back(new Pipe(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::mario:
                    m_actors.push_back(new Mario(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                case Level::piranha:
                    m_actors.push_back(new Piranha(this, i * SPRITE_WIDTH, j * SPRITE_HEIGHT));
                    break;
                }
                
            }
        }
       
    }
    return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (!isEnded()) {
        ostringstream oss;
        oss.fill('0');
        oss << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << setw(6) << getScore();
        if (m_player->hasJump()) {
            oss << " JumpPower!";
        }
        if (m_player->hasShoot()) {
            oss << " ShootPower!";
        }
        if (m_player->hasStar()) {
            oss << " StarPower!";
        }
        setGameStatText(oss.str());
        m_player->doSomething();

        for (int i = 0; i < m_actors.size(); ) {
            if (m_actors[i]->isAlive()) {
                m_actors[i]->doSomething();
                i++;
            }
            else {
                delete m_actors[i];
                m_actors.erase(m_actors.begin() + i);
            }
        }
        if (!m_player->isAlive()) {
            playSound(SOUND_PLAYER_DIE);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
       
    }
    else if (isEnded() && !hasWon()) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    else {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < m_actors.size(); i++) {
        delete m_actors[i];
    }
    m_actors.erase(m_actors.begin(), m_actors.end());
    delete m_player;
    m_player = nullptr;
}

bool StudentWorld::actorAt(int x, int y, Actor*& val) {

    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (((*it)->getX() == x) && (*it)->getY() == y) {
            val = *it;
            return true;
        }
        it++;

    }
    return false;

}
bool StudentWorld::isBlockingObjectAt(int x, int y) {
    
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (((*it)->getX() == x) && (*it)->getY() == y) {
            if (!(*it)->isPassable()) {
                return true;
            }
           
        }
        it++;

    }
    return false;
    
}

bool StudentWorld::isHitboxAt(int x, int y, Actor*& a) {

    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((((*it)->getX() + (SPRITE_WIDTH) >= x) && (*it)->getX() <= x) && (((*it)->getY() + (SPRITE_HEIGHT) >= y) && (*it)->getY() <= y)) {
           
                a = *it;
                return true;
           
            
        }
        it++;

    }
    return false;

}

bool StudentWorld::overlap(Actor* a, Actor* b) {
    if ((a->getX() >= b->getX() && a->getX() <= b->getX() + (SPRITE_WIDTH - 1)) || (b->getX() >= a->getX() && b->getX() <= a->getX() + (SPRITE_WIDTH - 1)))
    {
        if ((a->getY() >= b->getY() && a->getY() <= b->getY() + (SPRITE_HEIGHT - 1)) || (b->getY() >= a->getY() && b->getY() <= a->getY() + (SPRITE_HEIGHT - 1))) {
            return true;
        }
        else {
            return false;
        }
       

    }
    else {
        return false;
    }

}

