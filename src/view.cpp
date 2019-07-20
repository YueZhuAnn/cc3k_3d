#include <iostream>
#include <vector>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "view.hpp"

using namespace std;
using namespace glm;

#if DEBUG_INFO
    // any debug global here
#endif

const vec3 z_axis(0.0f,1.0f,0.0f);

// debug function
void printDrawInfo(vector< vector<Floor> > &drawInfo){
    for(auto r = drawInfo.begin(); r != drawInfo.end(); ++r){
        for(auto c = r->begin(); c != r->end(); ++c){
            Floor floor = *c;
            switch (floor){
                case Floor::wall:       cout << "X"; break;
                case Floor::tile:       cout << "."; break;
                case Floor::character:  cout << "c"; break;
                default: cout << " "; break;
            }
        }
        cout << endl;
    }
}

void printMat4(mat4 &matrix){
    for(unsigned x = 0; x < 4; x++){
        for(unsigned y = 0; y < 4; y++){
            y == 0? cout << "" : cout << " ";
            cout << matrix[x][y]; 
        }
        cout << endl;
    }
    cout << "************************" << endl;
}
/***********************************************************************/

CC3KView::CC3KView(CC3KModel *model, unsigned size):m_model(model), m_width(800), m_height(600), m_gridsize(size),
                                                    m_camera(vec3{0.0f, 2.0f, 3.0f}), m_background(vec3{0.2f, 0.3f, 0.3f}){}

CC3KView::~CC3KView(){}

void CC3KView::init(){
    const string shaderPath = "../shader/";
    const string vsBase = "VertexShader.vs";
    const string fsBase = "FragmentShader.fs";
    // Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

    // Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
    view = m_camera.getViewMatrix();
	proj = glm::perspective( 
		glm::radians( 30.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );

    // register for key callback
    void processInput();

    // init the Wall
    m_wall.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_wall.registerUniform();
    m_wall.uploadModel();           

    // init the floor
    m_tile.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_tile.registerUniform();
    m_tile.uploadModel(); 

    // init the characters
    m_enemy.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_enemy.registerUniform();
    m_enemy.uploadModel();  

    // init the player
    m_player.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_player.registerUniform();
    m_player.uploadModel(); 

    // init the potion
    m_potion.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_potion.registerUniform();
    m_potion.uploadModel();

    // init the gold
    m_gold.init((shaderPath+vsBase).c_str(), 
                (shaderPath+fsBase).c_str());
    m_gold.registerUniform();
    m_gold.uploadModel();    
}

void CC3KView::setUp(){
    glClearColor(m_background[0], m_background[1], m_background[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void CC3KView::viewRun(){
    windowCreation();
    init();
    run();
}

void CC3KView::gameOver(){ m_paused = true; }

void CC3KView::uploadWallUniform(){
    m_wall.uploadUniform("P", proj);
    m_wall.uploadUniform("V", view);
    m_wall.uploadUniform("colour", vec3(1.0f));
}

void CC3KView::uploadTileUniform(){
    m_tile.uploadUniform("P", proj);
    m_tile.uploadUniform("V", view);
    m_tile.uploadUniform("colour", vec3(0.0f, 0.0f, 0.5f));
}

void CC3KView::uploadCharaUniform(){
    m_enemy.uploadUniform("P", proj);
    m_enemy.uploadUniform("V", view);
    m_enemy.uploadUniform("colour", vec3(0.6f, 0.0f, 0.0f));
}

void CC3KView::uploadPlayerUniform(){
    m_player.uploadUniform("P", proj);
    m_player.uploadUniform("V", view);
    m_player.uploadUniform("colour", vec3(0.0f, 0.6f, 0.0f));
}

void CC3KView::uploadPotionUniform(){
    m_potion.uploadUniform("P", proj);
    m_potion.uploadUniform("V", view);
    m_potion.uploadUniform("colour", vec3(0.3f, 0.3f, 0.0f));
}

void CC3KView::uploadGoldUniform(){
    m_gold.uploadUniform("P", proj);
    m_gold.uploadUniform("V", view);
    m_gold.uploadUniform("colour", vec3(1.0f, 1.0f, 0.0f));
}

void CC3KView::processInput(){
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS){
        glfwSetWindowShouldClose(m_window, true);}

    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS){
        m_model->playerMove(-1, 0);}

    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        m_model->playerMove(1, 0);}

    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS){
        m_model->playerMove(0, -1);}

    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS){
        m_model->playerMove(0, 1);}

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS){
        m_model->playerAttack();}
}

void CC3KView::guiLogic(){
    bool showGameInfo = true;
    ImGui::Begin("Game Info", &showGameInfo);
    // Game Info
    {
    ImGui::Text( "Floor: %u",  m_model->getFloor());
    Species player_species = m_model->getPlayerSpecies();
    string sSpecies;
    switch(player_species){
        case Species::shade: sSpecies = "shade"; break;
        default: break;
    }
    ImGui::Text( "Player: %s",  sSpecies.c_str());
    unsigned player_hp = m_model->getPlayerHP();
    unsigned player_atk = m_model->getPlayerAtk();
    unsigned player_def = m_model->getPlayerDef();
    unsigned player_score = m_model->getScore();
    ImGui::Text( "HP: %u    Atk: %u   Def: %u   Score: %u", 
            player_hp, player_atk, player_def, player_score);
    }
    if (m_paused){
        ImGui::Text( "Game Over");
        if( ImGui::Button( "Restart Game" ) ) {
			m_model->gameRestart();
            m_paused = false;
		}
    }
    // Game Control
    {
        if( ImGui::Button( "Quit Game" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
    }
    ImGui::End();
}

void CC3KView::draw(){
    vector< vector<Floor> > drawInfo;
    m_model->fillDrawInfo(drawInfo);
#if DEBUG_INFO
    printDrawInfo(drawInfo);
#endif
    // upload uniforms
    uploadWallUniform();
    uploadTileUniform();
    uploadCharaUniform();
    uploadPlayerUniform();
    uploadPotionUniform();
    uploadGoldUniform();

    // draw 
    mat4 S = scale(mat4(1.0f), vec3(1.0f)/(float)(m_gridsize/2));
    mat4 R = mat4(1.0f);
    for(unsigned r = 0; r < m_gridsize; r++){
        for(unsigned c = 0; c < m_gridsize; c++){
            mat4 T = translate(mat4(1.0f), vec3((float)r-(m_gridsize/2), 0.0f,(float)c-(m_gridsize/2)));
            mat4 M = S*R*T;
            switch( drawInfo[r][c] ){
                case Floor::wall: {
                    m_wall.uploadUniform("M", M);
                    m_wall.draw(); break;}
                case Floor::tile: {
                    m_tile.uploadUniform("M", M);
                    m_tile.draw(); break;}
                case Floor::character: {
                    m_tile.uploadUniform("M", M);
                    m_tile.draw(); 
                    m_enemy.uploadUniform("M", M);
                    m_enemy.draw(); break;}
                case Floor::player: {
                    m_tile.uploadUniform("M", M);
                    m_tile.draw(); 
                    m_player.uploadUniform("M", M);
                    m_player.draw(); break;}
                case Floor::potion:{
                    m_tile.uploadUniform("M", M);
                    m_tile.draw(); 
                    m_potion.uploadUniform("M", M);
                    m_potion.draw(); break;}
                case Floor::gold:{
                    m_tile.uploadUniform("M", M);
                    m_tile.draw(); 
                    m_gold.uploadUniform("M", M);
                    m_gold.draw(); break;}
                default: break;
            }
        }
    }
    glBindVertexArray( 0 );
}
