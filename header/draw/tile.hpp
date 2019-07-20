#pragma once
#include "drawing.hpp"

class Tile: public Drawing{
public:
    Tile();
    ~Tile();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};