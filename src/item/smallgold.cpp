#include "smallgold.hpp"
#include "player.hpp"

using namespace std;

SmallGold::SmallGold(int index, Pos pos):Item(ItemTypes::goldNormal, pos, index){}

SmallGold::~SmallGold(){}

bool SmallGold::effect(Player *player){
    player->addScore(1);  
    return true;
}