#include "ph.hpp"
#include "player.hpp"

using namespace std;

PH::PH(int index, Pos pos):Item(ItemTypes::potionPH, pos, index){}

PH::~PH(){}

bool PH::effect(Player *player){
    player->subHP(10);
    return true;  
}