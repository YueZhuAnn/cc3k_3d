#include <iostream>
#include "cc3k.hpp"

using namespace std;

CC3K::CC3K(){
    
}

CC3K::~CC3K(){

}

int CC3K::gameInitialize(){
    // create model
    model = new CC3KModel("../src/map");
    if (model == nullptr){
        return -1;
    }
    unsigned gridsize = model->getGridSize();
    cout << "maze size: " << gridsize << "*" << gridsize << endl;

    // create window
    view = new CC3KView(model, gridsize);
    if (view == nullptr){
        return -1;
    }
    model->gameInit();
    model->linkView(view);
    view->viewRun();

    // window is closed, clear data
    model->clearData();
    delete model;
    delete view;

    return 0;
}