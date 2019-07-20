#pragma once
#include "drawing.hpp"

class Wall: public Drawing{
public:
    Wall();
    ~Wall();
    virtual void registerUniform() override;
    virtual void uploadModel() override;
};