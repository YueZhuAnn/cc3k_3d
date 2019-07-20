#include "item.hpp"

using namespace std;

Item::Item(ItemTypes type, Pos pos, unsigned index, string name)
    :m_type(type), m_pos(pos), m_index(index), m_name(name){}

Item::~Item(){}

Pos Item::getPos(){ return m_pos; }

ItemTypes Item::getType(){ return m_type; }