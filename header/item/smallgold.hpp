#pragma once
#include "item.hpp"

class SmallGold: public Item{
public:
    SmallGold(int index, Pos pos);
    ~SmallGold();
    virtual bool effect(Player *player) override;
};
