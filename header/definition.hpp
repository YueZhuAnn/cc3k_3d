#pragma once

enum Floor {
    none = 0,
    wall,
    tile,
    door,
    player,
    character,
    potion,
    gold
};

enum Direction {
    ver = 0,
    hor
};

enum Species {
    shade = 0,
    drow,
    vampire,
    troll,
    goblin,
    human,
    dwarf,
    elf,
    orc,
    merchant,
    dragon,
    halfling
};

enum ItemTypes{
    potionRH = 0,
    potionBA,
    potionBD,
    potionPH,
    potionWA,
    potionWD,
    goldNormal,
    goldSmall,
    goldDragon
};

struct Pos{
    int x;
    int y;
    Pos operator+(const Pos &rhs) const{
        return Pos{x+rhs.x, y+rhs.y};
    }
    Pos operator-(const Pos &rhs) const{
        return Pos{x-rhs.x, y-rhs.y};
    }
};