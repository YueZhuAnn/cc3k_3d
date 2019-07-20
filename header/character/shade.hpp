#pragma once
#include "player.hpp"

class Shade: public Player{
public:
    Shade(Pos pos, CC3KModel *model, Room *room);
    virtual ~Shade() override;
    // attack
    virtual void attacked(Human *enemy) override;
    virtual void attack(Enemy *enemy) override;

    virtual Species getSpecies() override;
};