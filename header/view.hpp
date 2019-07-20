#pragma once
#include <glm/ext.hpp>
#include "model.hpp"
#include "window.hpp"
#include "objects.hpp"
#include "camera.hpp"

class CC3KView: public Window{
// OpenGL related
    // Matrices controlling the camera and projection.
    Camera m_camera;
	glm::mat4 proj;
	glm::mat4 view;

    // drawing objects
    Wall m_wall;
    Tile m_tile;
    DrawEnemy m_enemy;
    DrawPlayer m_player;
    DrawPotion m_potion;
    DrawGold m_gold;

    // upload uniforms
    void uploadWallUniform();
    void uploadTileUniform();
    void uploadCharaUniform();
    void uploadPlayerUniform();
    void uploadPotionUniform();
    void uploadGoldUniform();

    // The model of the maze
    float m_shape_size = 0.5f;
    virtual void guiLogic() override;
    virtual void draw() override;
    virtual void init() override;
    virtual void setUp()override;
// related to game
    CC3KModel *m_model;
    const unsigned m_width;
    const unsigned m_height;
    const unsigned m_gridsize;
    glm::vec3      m_background;
    virtual void processInput() override;

public:
    CC3KView(CC3KModel *model, unsigned size);
    ~CC3KView();
    void viewRun();
    void gameOver();
};