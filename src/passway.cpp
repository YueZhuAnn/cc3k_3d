#include <iostream>
#include "passway.hpp"
#include "playerlist.hpp"

using namespace std;

Passway::Passway(Pos pos, Direction dir, unsigned length):
        pos(pos), dir(dir), length(length){}

Passway::~Passway(){}

bool Passway::validPos(const Pos &pos){
    int x = pos.x;
    int y = pos.y;
    if(x < 0 || y < 0) return false;
    if( dir == Direction::ver){
        return x == 0 && y < length;
    }
    return y == 0 && x < length;
}

bool Passway::validGlobalPos(const Pos &gpos ){
    Pos lpos = getLocalPos(gpos);
    return validPos(lpos); 
}

Pos Passway::getLocalPos(const Pos &gpos){ return gpos-pos; }

Pos Passway::getGlobalPos(const Pos &lpos){ return pos+lpos; }

void Passway::removePlayer(){ m_player = nullptr; }

void Passway::setPlayer(Player *player){ m_player = player; }

void Passway::fillDrawInfo(std::vector< std::vector<Floor> > &info){
    unsigned x = pos.x;
    unsigned y = pos.y;
    for(unsigned i = 0; i < length; i++){
        unsigned drawx = x;
        unsigned drawy = y;
        dir == Direction::ver? drawy += i :drawx += i;
        if(info[drawx][drawy] != Floor::none) continue;
        info[drawx][drawy] = Floor::tile;
    }
}