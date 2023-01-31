#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  void addActor(Actor* a);
  void deleteActor(Actor* a);

  bool actorAt(int x, int y, Actor*& temp);
  bool isBlockingObjectAt(int x, int y);
  bool isHitboxAt(int x, int y, Actor*& a);
  bool overlap(Actor* a, Actor* b);

  Peach* getPlayer();

  bool isEnded();
  void setEnded(bool val);
  bool hasWon();
  void setWon(bool val);

private:
	std::vector<Actor*> m_actors;
	Peach* m_player;
	bool m_isEnded;
	bool m_hasWon;

};



#endif // STUDENTWORLD_H_
