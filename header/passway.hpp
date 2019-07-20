#pragma once
#include <vector>
#include "definition.hpp"

class Player;

class Passway{
    Player *m_player = nullptr;
    const Pos pos;
    Direction dir;
    unsigned length;
public:
    Passway(Pos pos, Direction dir, unsigned length);
    ~Passway();
    bool validPos(const Pos &pos);
    bool validGlobalPos(const Pos &gpos);
    Pos getLocalPos(const Pos &gpos);
    Pos getGlobalPos(const Pos &lpos);
    void removePlayer();
    void setPlayer(Player *player);
    void fillDrawInfo(std::vector< std::vector<Floor> > &info);
};
