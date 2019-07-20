#pragma once
#include <vector>
#include "character.hpp"
#include "passway.hpp"
#include "enemylist.hpp"

class Passway;
class CC3KModel;
class Enemy;

class Player: public Character{
    Passway *m_passway = nullptr;
    const unsigned maxHP;
    const unsigned prevAtk;
    const unsigned prevDef;
    unsigned m_score = 0;
protected:
    CC3KModel *m_model;
public:
    Player(Species species, Pos pos, CC3KModel *model, Room *room, 
            unsigned HP, unsigned Atk, unsigned Def);
    virtual ~Player() = 0;
    // attack
    void attackEnemy(std::vector<Enemy *> &enemies);
    virtual void attacked(Human *enemy);
    virtual void attacked(Dragon *enemy);
    virtual void attack(Enemy *enemy);

    // effect
    void addHP(const unsigned HP);
    void addAtk(const unsigned Atk);
    void addDef(const unsigned Def);

    void subHP(const unsigned HP);
    void subAtk(const unsigned Atk);
    void subDef(const unsigned Def);

    void addScore(const unsigned score);

    // get info
    Pos getGlobalPos();
    void resetRoom(Room *room, const Pos &newpos);
    void resetPassway(Passway *passway, const Pos &newpos);
    void AtkDefReset();
    unsigned getScore();
};