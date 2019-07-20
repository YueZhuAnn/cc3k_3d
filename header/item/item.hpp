#pragma once
#include <string>
#include "definition.hpp"

class Player;

class Item{
    // characteristics
    ItemTypes m_type;
    std::string m_name;
protected:
    const int m_index;
    Pos m_pos;
public:
    Item(ItemTypes type, Pos pos, unsigned index, std::string name = "");
    virtual ~Item() = 0;
    virtual bool effect(Player *player) = 0;
    Pos getPos();
    ItemTypes getType();
};