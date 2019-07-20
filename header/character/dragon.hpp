#pragma once
#include "enemy.hpp"

class DragonGold;

class Dragon: public Enemy{
    DragonGold *m_gold = nullptr;
public:
    Dragon(Pos pos, Room *room, int index);
    virtual ~Dragon() override;
    void setGold(DragonGold *gold);
    virtual Pos generateNewPos() override;
    // attack
    virtual void attack(Player *player) override;
    virtual void attacked(Shade *player) override;

    // get Info
    virtual Species getSpecies() override;
};