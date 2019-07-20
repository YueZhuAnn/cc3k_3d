#pragma once
#include "enemy.hpp"

class Human: public Enemy{
public:
    Human(Pos pos, Room *room, int index);
    virtual ~Human() override;
    
    // attack
    virtual void attack(Player *player) override;
    virtual void attacked(Shade *player) override;

    // get Info
    virtual Species getSpecies() override;
};