#pragma once
#include "item.hpp"

class WA: public Item{
public:
    WA(int index, Pos pos);
    ~WA();
    virtual bool effect(Player *player) override;
};