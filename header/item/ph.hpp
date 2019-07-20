#pragma once
#include "item.hpp"

class PH: public Item{
public:
    PH(int index, Pos pos);
    ~PH();
    virtual bool effect(Player *player) override;
};