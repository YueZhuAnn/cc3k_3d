#pragma once
#include "drawing.hpp"

class DrawEnemy: public Drawing{
public:
    DrawEnemy();
    ~DrawEnemy();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};