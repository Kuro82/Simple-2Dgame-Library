#ifndef GL_GAME_SCENE
#define GL_GAME_SCENE

#include"GLgameCore.hpp"

enum SceneType {
    MENU_SCENE = 0,
    OPENING_SCENE = 1,
    STAGE_SCENE = 2,
};

static void setNextSceneCore(SceneType type, SceneManager* manager);
void setNextScene(SceneType type, SceneManager* manager){
    setNextSceneCore(type, manager);
}

#include"Scenes/Menu.hpp"
#include"Scenes/Opening.hpp"
#include"Scenes/Stage.hpp"

static void setNextSceneCore(SceneType type, SceneManager* manager){
    switch(type){
        case MENU_SCENE:
            manager->setNextScene(new Menu());
            break;
        case OPENING_SCENE:
            manager->setNextScene(new Opening());
            break;
        case STAGE_SCENE:
            manager->setNextScene(new Stage());
            break;
        default:
            break;
    }
}

//This is basic scene.
//You can make other scene functions like Basic scene's.
class Basic : public Scene{
    std::vector<GLsprite> spr;
    public:
        ~Basic(){
            int size=spr.size();
            for(int i=0;i<size;i++){
                spr[i].deleteSprite();
            }
        }
        void idle(){}
        void display(){}
        void key_callback(int key, int scancode, int action, int mods);
        void init(){}
};

#endif