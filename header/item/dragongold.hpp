#pragma once
#include "item.hpp"

class Dragon;
class Player;

class DragonGold: public Item{
    Dragon *m_dragon = nullptr;
public:
    DragonGold(int index, Pos pos);
    ~DragonGold();
    virtual bool effect(Player *player) override;
    void setDragon(Dragon *dragon);
    void guard(Player *player);
    void removeDragon();
};
