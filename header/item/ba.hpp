#pragma once
#include "item.hpp"

class BA: public Item{
public:
    BA(int index, Pos pos);
    ~BA();
    virtual bool effect(Player *player) override;
};