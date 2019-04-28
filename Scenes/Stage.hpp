#ifndef GL_GAME_SCENE_STAGE
#define GL_GAME_SCENE_STAGE

#include"../Scenes.hpp"
#include"../libs/mylibs/Csv.hpp"

using namespace std;

enum Direction {
    D_LEFT=0, D_UP=1, D_RIGHT=2, D_DOWN=3,
};

class Map{
        class MapChip{
                int graphicType;
                int collisionType;
            public:
                MapChip(){}
                MapChip(int graphic, int collision) : graphicType(graphic), collisionType(collision){}
                MapChip(const MapChip &mapChip): graphicType(mapChip.graphicType), collisionType(mapChip.collisionType){}
                MapChip &operator=(const MapChip &root){
                    graphicType = root.graphicType;
                    collisionType = root.graphicType;
                    return (*this);
                }
                void setGraphic(int graphic){ graphicType = graphic; }
                void setCollision(int collision){ collisionType = collision; }
                int getGraphic(){ return graphicType; }
                int getCollision(){ return collisionType; }
        };

        int chipSize;
        std::vector<MapChip> chips;
        int mapWidth, mapHeight;
        GLsprite graphic;

        void createMap(int width, int height, std::vector<int> graphicNums, std::vector<int> collisionNums){
            mapWidth = width, mapHeight = height;
            chips.resize(mapWidth * mapHeight);
            for(int y=0;y<mapHeight;y++){
                for(int x=0;x<mapWidth;x++){
                    chips[mapWidth*y + x].setGraphic(graphicNums[mapWidth*y + x]);
                    chips[mapWidth*y + x].setCollision(collisionNums[mapWidth*y + x]);
                }
            }

            int mapChipNum = 6;
            graphic = GLsprite(string("Image/map/blackchips.png"), mapChipNum, 1, 1.0);
            graphic.setAllSize(chipSize, chipSize);
        }
    public:
        Map(){ chipSize=60; }
        Map(int width, int height, std::vector<int> graphic, std::vector<int> collision, int chipSize=60): chipSize(chipSize){
            createMap(width, height, graphic, collision);
        }
        Map(std::vector<std::vector<std::string> > graphic, std::vector<std::vector<std::string> > collision, int chipSize=60): chipSize(chipSize){
            int width = graphic[0].size(), height = graphic.size();
            std::vector<int> graphicVector(width * height);
            std::vector<int> collisionVector(width * height);
            for(int y=0;y<height;y++){
                for(int x=0;x<width;x++){
                    graphicVector[y*width + x] = std::stoi(graphic[y][x]);
                    collisionVector[y*width + x] = std::stoi(collision[y][x]);
                }
            }
            createMap(width, height, graphicVector, collisionVector);
        }
        Map &operator=(const Map &root){
            mapWidth = root.mapWidth;
            mapHeight = root.mapHeight;
            chips = std::move(root.chips);
            graphic = std::move(root.graphic);
            return (*this);
        }
        MapChip access(int col, int row){
            return chips[mapWidth*row + col];
        }
        bool checkHitCollision(int x, int y){
            int tx = abs( (x + chipSize*mapWidth/2)/chipSize );
            int ty = abs( mapHeight - 1 - (y + chipSize*mapHeight/2)/chipSize );
            if( chips[ty*mapWidth + tx].getCollision() == 0 ){
                return true;
            }
            return false;
        }
        void draw(){
            graphic.setInt(0, 't');
            for(int y=0;y<mapHeight;y++){
                for(int x=0;x<mapWidth;x++){
                    graphic.setInt((access(x, mapHeight - y - 1)).getGraphic(), 't');
                    graphic.setPosition(chipSize*x - chipSize*(mapWidth/2) + chipSize/2, chipSize*y - chipSize*(mapHeight/2) + chipSize/2);
                    graphic.draw();
                }
            }
        }
        void draw(int width, int height){
            graphic.setInt(0, 't');
            for(int y=0;y<mapHeight;y++){
                for(int x=0;x<mapWidth;x++){
                    graphic.setInt((access(x, mapHeight - y - 1)).getGraphic(), 't');
                    graphic.setPosition(chipSize*x - chipSize*(mapWidth/2) + chipSize/2, chipSize*y - chipSize*(mapHeight/2) + chipSize/2);
                    graphic.draw();
                }
            }
        }
};

class Hero{
        GLsprite sprite;
        int chipSize;
        bool moveFlag[4];
        double position[2];
    public:
        Hero(int chipSize=60): chipSize(chipSize){
            sprite = GLsprite(std::string("Image/Hero.png"), 1.0);
            sprite.setSize(chipSize, chipSize*2);
            moveFlag[0] = moveFlag[1] = moveFlag[2] = moveFlag[3] = false;
            position[0] = position[1] = 0;
        }
        ~Hero(){}
        Hero &operator=(const Hero &root){
            for(int i=0;i<4;i++){ moveFlag[i] = root.moveFlag[i]; }
            for(int i=0;i<2;i++){ position[i] = root.position[i]; }
            sprite = std::move(root.sprite);
            return (*this);
        }
        void setMoveFlag(Direction moveDirection, bool flag){
            moveFlag[moveDirection] = flag;
        }
        void update(Map &map){
            double movedX = position[0], movedY = position[1];

            // X
            double diffX = 4;
            if( moveFlag[D_LEFT] && !(map.checkHitCollision(movedX-chipSize/2-diffX, movedY+1-chipSize))
                && !(map.checkHitCollision(movedX-chipSize/2-diffX, movedY-1)) ){
                    movedX -= diffX;
            }
            if( moveFlag[D_RIGHT] && !(map.checkHitCollision(movedX+chipSize/2+diffX, movedY+1-chipSize))
                && !(map.checkHitCollision(movedX+chipSize/2+diffX, movedY-1)) ){
                movedX += diffX;
            }
            position[0] = movedX;

            // Y
            double diffY = 2;
            if( moveFlag[D_UP] && !(map.checkHitCollision(position[0]-chipSize/2+1, movedY+chipSize/4+diffY*5))
                && !(map.checkHitCollision(position[0]+chipSize/2-1, movedY+chipSize/4+diffY*3)) ){
                movedY += diffY*5;
            }
            if( moveFlag[D_DOWN] && !(map.checkHitCollision(position[0]-chipSize/2+1, movedY-chipSize-diffY))
                && !(map.checkHitCollision(position[0]+chipSize/2-1, movedY-chipSize-diffY)) ){
                movedY -= diffY;
            }
            if( !(map.checkHitCollision(position[0]-chipSize/2+1, movedY-chipSize-diffY*2))
                && !(map.checkHitCollision(position[0]+chipSize/2-1, movedY-chipSize-diffY*2)) ){
                movedY -= diffY*2;
            }
            position[1] = movedY;

            sprite.setPosition((int)(position[0]), (int)(position[1]));
        }
        void draw(){
            sprite.draw();
        }
        void setPosition(double x, double y){
            position[0] = x;
            position[1] = y;
        }
        void getPosition(double &x,double &y){
            x = position[0];
            y = position[1];
        }
        GLsprite getSprite(){
            return sprite;
        }
};

class Stage : public Scene{
        const int CHIPSIZE = 60;
        GLsprite mapchips;
        Map stageMap;
        Hero hero;

    public:
        ~Stage(){
            mapchips.deleteSprite();
            (hero.getSprite()).deleteSprite();
        }
        void idle(){
            hero.update(stageMap);
        }
        void display(){
            double heroX, heroY;
            hero.getPosition(heroX, heroY);
            int windowWidth, windowHeight;
            manager->getWindowSize(&windowWidth, &windowHeight);

            glPushMatrix();
            glTranslated(-heroX, 0, 0);
            stageMap.draw(windowWidth, windowHeight);
            hero.draw();
            glPopMatrix();
        }
        void key_callback(int key, int scancode, int action, int mods){
            if( KEY(Q) && ACT(PRESS) ){
                exitGame();
            }

            if( KEY(SPACE) && ACT(PRESS) ){
                setNextScene(OPENING_SCENE, manager);
            }

            //for hero
            if( ACT(PRESS) ){
                if( KEY(LEFT) ){ hero.setMoveFlag(D_LEFT, true); }
                if( KEY(RIGHT) ){ hero.setMoveFlag(D_RIGHT, true); }
                if( KEY(UP) ){
                    manager->audio.playAudio(2);
                    hero.setMoveFlag(D_UP, true);
                }
                if( KEY(DOWN) ){ hero.setMoveFlag(D_DOWN, true); }
            }
            if( ACT(RELEASE) ){
                if( KEY(LEFT) ){ hero.setMoveFlag(D_LEFT, false); }
                if( KEY(RIGHT) ){ hero.setMoveFlag(D_RIGHT, false); }
                if( KEY(UP) ){ hero.setMoveFlag(D_UP, false); }
                if( KEY(DOWN) ){ hero.setMoveFlag(D_DOWN, false); }
            }
        }
        void init(){
            //load image
            hero = std::move(Hero(CHIPSIZE));

            //load map data
            Csv graphic("Data/testStage3_graphic.csv");
            Csv collision("Data/testStage3_collision.csv");
            stageMap = std::move(Map(graphic.getDataBase(), collision.getDataBase()));
        }
};


#endif