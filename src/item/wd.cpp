#include "wd.hpp"
#include "player.hpp"

using namespace std;

WD::WD(int index, Pos pos):Item(ItemTypes::potionWD, pos, index){}

WD::~WD(){}

bool WD::effect(Player *player){
    player->subDef(5);  
    return true;
}