#pragma once
#include "item.hpp"

class WD: public Item{
public:
    WD(int index, Pos pos);
    ~WD();
    virtual bool effect(Player *player) override;
};