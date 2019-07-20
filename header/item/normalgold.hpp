#pragma once
#include "item.hpp"

class NormalGold: public Item{
public:
    NormalGold(int index, Pos pos);
    ~NormalGold();
    virtual bool effect(Player *player) override;
};
