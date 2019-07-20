#pragma once
#include "view.hpp"
#include "model.hpp"

class CC3K{
    CC3KView *view;
    CC3KModel *model;
public:
    CC3K();
    ~CC3K();
    int gameInitialize();
};