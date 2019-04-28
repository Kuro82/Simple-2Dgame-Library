#ifndef GL_GAME_SCENE_MENU
#define GL_GAME_SCENE_MENU

#include"../Scenes.hpp"

using namespace std;

class Menu : public Scene{
        vector<GLsprite> spr;
    public:
        ~Menu(){
            int size=spr.size();
            for(int i=0;i<size;i++){
                spr[i].deleteSprite();
            }
        }
        void idle(){}
        void display(){}
        void key_callback(int key, int scancode, int action, int mods){
            if( KEY(Q) && ACT(PRESS) ){
                glfwTerminate();
                exit(0);
            }

            if( KEY(W) && ACT(PRESS) ){
                setNextScene(OPENING_SCENE, manager);
            }
        }
        void init(){}
};

#endif