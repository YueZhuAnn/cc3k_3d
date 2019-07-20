#include "enemy.hpp"

using namespace std;

Enemy::Enemy(Species species, Pos pos, Room *room, int index,
        unsigned HP, unsigned Atk, unsigned Def):
        Character(species, pos, room, HP, Atk, Def),
        index(index){}

Enemy::~Enemy(){}

void Enemy::attack(Player *player){}

void Enemy::attacked(Shade *player){}