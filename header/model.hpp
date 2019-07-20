#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include "definition.hpp"
#include "room.hpp"
#include "passway.hpp"

class CC3KView;

class CC3KModel{
    CC3KView *m_view = nullptr;
    std::thread *m_thread = nullptr;
    const std::string m_mapPath;
    unsigned m_size = 0;
    unsigned m_roomsize = 0;
    unsigned m_enemies = 20;
    unsigned m_potions = 10;
    unsigned m_floor    = 0;
    std::vector<Species> enemy_prob;
    std::vector<ItemTypes> potion_prob;
    std::vector<ItemTypes> gold_prob;
    std::vector<Room*> rooms;
    std::vector<Passway*> passways;
    bool bRunning = true;

    // player
    Player *m_player = nullptr;

    // enemy
    Enemy *m_enemy = nullptr;
    void readRoomInfo(std::ifstream &ifsFile);
    void readPasswayInfo(std::ifstream &ifsFile);
    void generateFloor();
    void generatePlayer();
    void moveEnemies();
    void moveLoop();
    void updatePlayerPos(int x, int y);
    void resetFloor();
public:
    CC3KModel(std::string mapPath);
    ~CC3KModel();
    void linkView(CC3KView *view);
    void clearData();
    void gameInit();
    void gameOver();
    void gameRestart();
    void playerMove(int x, int y);
    void fillDrawInfo(std::vector< std::vector<Floor> > &info);
    unsigned getGridSize();
    unsigned getFloor();

    // battle
    void playerAttack();
    
    // get player info
    Species getPlayerSpecies();
    unsigned getPlayerHP();
    unsigned getPlayerAtk();
    unsigned getPlayerDef();
    unsigned getScore();
    
    // get enemy info
    Species getEnemySpecies();
    unsigned getEnemyHP();
    unsigned getEnemyAtk();
    unsigned getEnemyDef();
};