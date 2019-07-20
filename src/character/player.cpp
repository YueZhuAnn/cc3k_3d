#include "player.hpp"
#include "room.hpp"
#include "model.hpp"
#include <iostream>

using namespace std;

Player::Player(Species species, Pos pos, CC3KModel *model, Room *room,
        unsigned HP, unsigned Atk, unsigned Def):
        Character(species, pos, room, HP, Atk, Def), 
        m_model(model), maxHP(HP), prevAtk(Atk), prevDef(Def){}

Player::~Player(){}

void Player::attackEnemy(std::vector<Enemy *> &enemies){
        auto curtime = chrono::high_resolution_clock::now();
        auto t_interval = chrono::duration_cast<chrono::milliseconds>(curtime  - m_time);
        if(t_interval.count() < 500) return;
        // update time
        m_time = chrono::high_resolution_clock::now();
        // attack each enemy
        for(auto it = enemies.begin(); it != enemies.end(); ++it){
                attack(*it);
        }
}

void Player::attacked(Human *enemy){}

void Player::attacked(Dragon *enemy){
        float def = (float)100+m_Def;
        float atk = (float)enemy->getDef();
        int damage = 100/def * atk;
        m_HP -= min((int)m_HP, damage);
        if(m_HP == 0){ m_model->gameOver(); }
}

void Player::attack(Enemy *enemy){}

void Player::addHP(const unsigned HP){
    m_HP = min(maxHP, m_HP+HP);
}

void Player::addAtk(const unsigned Atk){
    m_Atk += Atk;
}

void Player::addDef(const unsigned Def){
    m_Def += Def;
}

void Player::subHP(const unsigned HP){
    m_HP -= min(HP, m_HP);
    if (m_HP == 0){
        // GAME OVER
        cout << "GAME OVER" << endl;
    }
}

void Player::subAtk(const unsigned Atk){ m_Atk -= min(m_Atk, Atk); }

void Player::subDef(const unsigned Def){ m_Def -= min(m_Def, Def); }

Pos Player::getGlobalPos(){
        if (m_room != nullptr){
                return m_room->getGlobalPos(m_pos);
        }
        return m_passway->getGlobalPos(m_pos);
}


void Player::addScore(const unsigned score){
    m_score += score;
}

void Player::resetRoom(Room* room, const Pos &newpos){
        if(m_passway) { m_passway->removePlayer(); }
        if(m_room) { m_room->removePlayer(); }
        m_passway = nullptr;
        m_room = room;
        m_pos = newpos;
}

void Player::resetPassway(Passway* passway, const Pos &newpos){
        if(m_passway) { m_passway->removePlayer(); }
        if(m_room) { m_room->removePlayer(); }
        m_room = nullptr;
        m_passway = passway;
        m_pos = newpos;
}

void Player::AtkDefReset(){
        m_Def = prevDef;
        m_Atk = prevAtk;
}

unsigned Player::getScore(){ return m_score; }