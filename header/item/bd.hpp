#pragma once
#include "item.hpp"

class BD: public Item{
public:
    BD(int index, Pos pos);
    ~BD();
    virtual bool effect(Player *player) override;
};