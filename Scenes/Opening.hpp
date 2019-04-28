#ifndef GL_GAME_SCENE_OPENING
#define GL_GAME_SCENE_OPENING

#include"../Scenes.hpp"

using namespace std;

class Opening : public Scene{
        vector<GLsprite> spr;
        Animator anime;
    public:
        Opening(){}
        ~Opening(){
            int size=spr.size();
            for(int i=0;i<size;i++){
                spr[i].deleteSprite();
            }
        }
        void idle(){
        }
        void display(){
            spr[0].draw();
        }
        void key_callback(int key, int scancode, int action, int mods){
            if( KEY(Q) && ACT(PRESS) ){
                exitGame();
            }

            if( KEY(SPACE) && ACT(PRESS) ){
                manager->audio.stopAudio(0);
                manager->audio.playAudio(1);
                setNextScene(STAGE_SCENE, manager);
            }
        }
        void init(){
            spr.push_back( GLsprite(string("Image/gamestart.png"), 1, 3, 1.0) );
            int w, h;
            manager->getWindowSize(&w, &h);
            spr[0].setSize(w, h/3);

            manager->audio.playAudio(0);
        }
};

#endif