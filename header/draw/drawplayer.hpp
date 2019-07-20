#pragma once
#include "drawing.hpp"

class DrawPlayer: public Drawing{
public:
    DrawPlayer();
    ~DrawPlayer();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};