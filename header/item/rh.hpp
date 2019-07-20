#pragma once
#include "item.hpp"

class RH: public Item{
public:
    RH(int index, Pos pos);
    ~RH();
    virtual bool effect(Player *player) override;
};