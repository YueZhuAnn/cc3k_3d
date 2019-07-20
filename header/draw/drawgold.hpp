#pragma once
#include "drawing.hpp"

class DrawGold: public Drawing{
public:
    DrawGold();
    ~DrawGold();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};