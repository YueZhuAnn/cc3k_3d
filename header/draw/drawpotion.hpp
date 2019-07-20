#pragma once
#include "drawing.hpp"

class DrawPotion: public Drawing{
public:
    DrawPotion();
    ~DrawPotion();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};