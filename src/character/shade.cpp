#include "shade.hpp"
#include "human.hpp"
#include "model.hpp"
#include <iostream>

using namespace std;

Shade::Shade(Pos pos, CC3KModel *model, Room *room):
        Player{Species::shade, pos, model, room, 125, 25, 25}{}

Shade::~Shade(){}

void Shade::attacked(Human *enemy){
        float def = (float)100+m_Def;
        float atk = (float)enemy->getDef();
        int damage = 100/def * atk;
        m_HP -= min((int)m_HP, damage);
        if( m_HP == 0 ){ m_model->gameOver(); }
}

void Shade::attack(Enemy *enemy){
        enemy->attacked(this);
}

Species Shade::getSpecies(){ return Species::shade; }