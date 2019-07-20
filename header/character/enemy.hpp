#pragma once
#include "character.hpp"

class Player;
class Shade;

class Enemy: public Character{
protected:
    const int index;
public:
    virtual void attack(Player *player);
    virtual void attacked(Shade *player);
    Enemy(Species species, Pos pos, Room *room, int index, 
    unsigned HP, unsigned Atk, unsigned Def);
    virtual ~Enemy() = 0;
};