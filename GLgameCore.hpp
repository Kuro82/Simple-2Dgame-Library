#ifndef GL_GAME_CORE
#define GL_GAME_CORE

//public
#include<iostream>
#include<vector>
//graphics
#include<memory>
#include<GLFW/glfw3.h>
#include"libs/stb-master/stb_image.h"
//audio
#include<OpenAL/al.h>
#include<OpenAL/alc.h>
#include<unistd.h>
#include<math.h>
#define DR_MP3_IMPLEMENTATION
#include"libs/dr_libs-master/dr_mp3.h"



/* ---------------  macro  ---------------- */
#define KEY(str) ( key == GLFW_KEY_ ## str )
#define ACT(str) ( action == GLFW_ ## str )

/* ---------------  tool functions  ---------------- */
namespace{
    int makeSurplus(int n, int divider){
        if( n<0 ){
            int positiveN = abs(n);
            n += (positiveN / divider + 1) * divider;
        }
        return (n % divider);
    }
};

/* ---------------  2D graphics class  ---------------- */
class GLspriteBase{
        int w, h, bpp;
        double xscale, yscale;
        GLuint tex;
    public:
        GLspriteBase(){}
        GLspriteBase(std::string fname, double scale){
            xscale = yscale = scale;
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
            unsigned char *pix=nullptr;
            pix = stbi_load(fname.c_str(), &w, &h, &bpp, 4);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
            free(pix);
            glDisable(GL_BLEND);
        }
        GLspriteBase(std::vector<unsigned char> image, int iw, int ih, int ibpp, double scale){
            xscale = yscale = scale;
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
            w = iw, h = ih, bpp = ibpp;
            glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
            glDisable(GL_BLEND);
        }
        GLspriteBase(const GLspriteBase &root): w(root.w), h(root.h), bpp(root.bpp), xscale(root.xscale), yscale(root.yscale), tex(root.tex){}
        GLspriteBase &operator=(const GLspriteBase &root){
            w = root.w;
            h = root.h;
            bpp = root.bpp;
            xscale = root.xscale;
            yscale = root.yscale;
            tex = root.tex;
            return (*this);
        }
        ~GLspriteBase(){}
        void setScale(double xs, double ys=0){ xscale = xs; yscale = ys==0 ? xs : ys; }
        double getScale(char c){ return (c=='y' || c=='Y') ? yscale : xscale; }
        void setSize(int width, int height){ w=width; h=height; }
        int getInt(char c){
            int num=0;
            switch(c){
                case 'w': case 'W': num=w; break;
                case 'h': case 'H': num=h; break;
                default: break;
            }
            return num;
        }
        void setInt(int num, char c){
            switch(c){
                case 'w': case 'W': w=num; break;
                case 'h': case 'H': h=num; break;
                default: break;
            }
        }
        void draw(int x, int y){
            glEnable( GL_BLEND );
            glBindTexture(GL_TEXTURE_2D, tex);
            glBegin(GL_POLYGON);
                for(int i=0;i<4;i++){
                    glTexCoord2f( !(!(i%3)), 1-(i/2) );
                    glVertex2f(x + w*xscale*(!(!(i%3)) - 0.5), y + h*yscale*(i/2 - 0.5) );
                }
            glEnd();
            glDisable( GL_BLEND );
        }
        void deleteSpriteBase(){
            glDeleteTextures(1, &tex);
        }
};
class GLsprite{
        int x, y, texState;
        std::vector<GLspriteBase> sprites;
    public:
        GLsprite(){ init(); }
        GLsprite(std::string fileName, double scale=1.0){
            init();
            addTexture(fileName, scale);
        }
        GLsprite(std::string fileName, int xn, int yn, double scale=1.0){
            init();
            addDivideTexture(fileName, xn, yn, scale);
        }
        GLsprite(const GLsprite &root): x(root.x), y(root.y), texState(root.texState){
            sprites = std::move(root.sprites);
        }
        GLsprite &operator=(const GLsprite &root){
            x = root.x;
            y = root.y;
            texState = root.texState;
            sprites = std::move(root.sprites);
            return (*this);
        }
        ~GLsprite(){}
        void init(){ x = y = texState = 0; }
        void addTexture(std::string fname, double scale=1.0){
            sprites.push_back( GLspriteBase(fname, scale) );
            texState = sprites.size()-1;
        }
        void addDivideTexture(std::string fileName, int xn, int yn, double scale=1.0){
            int w, h, bpp;
            unsigned char *pix;
            pix = stbi_load(fileName.c_str(), &w, &h, &bpp, 4);
            if( w%xn!=0 || h%yn!=0 ){//alert Crach Danger
                printf("Danger : w:%d %% xn:%d = %d, h:%d %% yn:%d = %d\n", w, xn, w%xn, h, yn, h%yn);
            }
            std::vector<unsigned char> image;
            int wpxn = (int)(w/xn), hpyn = (int)(h/yn);
            image.reserve(wpxn*hpyn*bpp);
            image.resize(wpxn*hpyn*bpp);
            for(int i=0;i<yn;i++){
                for(int j=0;j<xn;j++){
                    //image devide
                    for(int iy=0;iy<hpyn;iy++){
                        for(int ix=0;ix<wpxn;ix++){
                            for(int bit=0;bit<bpp;bit++){
                                image[ (iy*wpxn + ix)*bpp + bit ] = pix[ (((i*hpyn) + iy)*w + (j*wpxn) + ix)*bpp + bit ];
                            }
                        }
                    }
                    sprites.push_back( GLspriteBase(image, wpxn, hpyn, bpp, scale) );
                }
            }
            free(pix);
            texState = sprites.size() - 1;
        }
        void deleteSprite(){
            int size=sprites.size();
            for(int i=0;i<size;i++){ sprites[i].deleteSpriteBase(); }
        }
        void setPosition(int xp, int yp){ x=xp; y=yp; }
        void addPosition(int xp, int yp){ x+=xp; y+=yp; }
        void addTexState(int add, int start=0, int goal=-1){
            int sprsize=sprites.size();
            if( goal<0 ){ goal = sprsize-1; }
            start = makeSurplus(start, sprsize);
            goal = makeSurplus(goal, sprsize);
            int length = (goal - start + sprsize) % sprsize;
            int diff = (texState - start + sprsize) % sprsize;
            texState = ((diff + add) % (length+1) + start) % sprsize;
        }
        int getInt(char c){
            int num=0;
            switch(c){
                case 'x': case 'X': num=x; break;
                case 'y': case 'Y': num=y; break;
                case 'w': case 'W': case 'h': case 'H': num=sprites[texState].getInt(c); break;
                case 't': case 'T': num=texState; break;
                default: break;
            }
            return num;
        }
        void setInt(int num, char c){
            switch(c){
                case 'x': case 'X': x=num; break;
                case 'y': case 'Y': y=num; break;
                case 'w': case 'W': case 'h': case 'H': sprites[texState].setInt(num, c); break;
                case 't': case 'T': if( num<sprites.size() ){ texState=num; } break;
                default: break;
            }
        }
        void setSize(int w, int h, int tex=-1){
            if(tex<0){ tex = texState; }
            sprites[tex].setSize(w, h);
        }
        void setAllSize(int w, int h){
            for(int i=0;i<sprites.size();i++){
                sprites[i].setSize(w, h);
            }
        }
        int getNumSprites(){ return sprites.size(); }
        void draw(){
            sprites[texState].draw(x, y);
        }
};
//Animator class is unstable.
class Animator{
        GLsprite sprite;
        int texStart, texGoal, freq, counter, texDiff;
        int timer;
    public:
        Animator(){}
        Animator(GLsprite spr, int updateFreq, int animationTimer=0, int diff=1, int start=0, int goal=-1){
            sprite = spr;
            texStart = start;
            texGoal = goal<0 ? spr.getNumSprites() : goal;
            texDiff = diff;
            freq = updateFreq;
            counter = 1;
            timer = animationTimer; //0 = continue forever, -1 = stop
        }
        void draw(){
            if(timer>=0){
                if(counter%freq==0){
                    sprite.addTexState(texDiff, texStart, texGoal);   
                }
                counter++;
                if(timer>0 && timer<counter){
                    timer = -1;
                }
            }
            sprite.draw();
        }
};

/* ---------------  audio Manager class  ---------------- */
class MP3Audio{
        std::string filename;
        ALuint buffer, source;
        float gain;
        bool onloop;
    public:
        MP3Audio(std::string fname, float t_gain=1.0f, bool loop=false) : filename(fname), gain(t_gain), onloop(loop) {
            buffer = source = 0;
            alGenBuffers(1, &buffer);
            alGenSources(1, &source);

            unsigned int total=0;
            drmp3 mp3;
            drmp3_init_file(&mp3, filename.c_str(), NULL);
            float* wave = drmp3__full_decode_and_close_f32(&mp3, NULL, (drmp3_uint64*)(&total));
            total *= mp3.channels;
            std::vector<short> wdata;
            wdata.resize(total);
            for(int i=0;i<total;i++){ wdata[i] = wave[i]*32767; }

            if( mp3.channels==1 ){
                alBufferData(buffer, AL_FORMAT_MONO16, &(wdata[0]), sizeof(short)*total, mp3.sampleRate);
            }else if( mp3.channels==2 ){
                alBufferData(buffer, AL_FORMAT_STEREO16, &(wdata[0]), sizeof(short)*total, mp3.sampleRate);
            }
            alSourcei(source, AL_BUFFER, buffer);
            alSourcef(source, AL_GAIN, gain);
            alSourcei(source, AL_LOOPING, onloop);
        }
        void setGain(float t_gain){ gain = t_gain; }
        void setLoop(bool loop){ onloop = loop; }
        ALuint getSource(){ return source; }
        void deleteAudio(){
            alDeleteBuffers(1, &buffer);
            alDeleteSources(1, &source);
        }
};
class AudioManager{
        ALCdevice *device;
        ALCcontext *context;
        std::vector<MP3Audio> audios;
        int cnt;
    public:
        AudioManager() : cnt(0) {
            device = alcOpenDevice(NULL);
            if(device){
                context = alcCreateContext(device, NULL);
                alcMakeContextCurrent(context);
            }
        }
        ~AudioManager(){
            int size=audios.size();
            for(int i=0;i<size;i++){
                audios[i].deleteAudio();
            }
            alcDestroyContext(context);
            alcCloseDevice(device);
        }
        void loadMP3(std::string filename){
            audios.push_back( MP3Audio(filename) );
            cnt = audios.size();
        }
        void playAudio(int num){
            if( num<cnt ){
                alSourceStop( audios[num].getSource() );
                alSourcePlay( audios[num].getSource() );
            }
        }
        void stopAudio(int num){
            if( num<cnt ){
                alSourceStop( audios[num].getSource() );
            }
        }
};

/* ---------------  game scene class  ---------------- */
class SceneManager;
class Scene{
    protected:
        SceneManager* manager;
    public:
        Scene(){}
        virtual ~Scene(){}
        virtual void idle(){}
        virtual void display(){}
        virtual void key_callback(int key, int scancode, int action, int mods){}
        virtual void init(){}
        void setManager(SceneManager* ptr){ manager = ptr; }
};
static std::shared_ptr<Scene> keyScene;
void exitGame(){
    glfwTerminate();
    exit(0);
}
void GLkeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    keyScene->key_callback(key, scancode, action, mods);
}
void resize(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width*2, height*2);
    glLoadIdentity();
    glOrtho(-width, width, -height, height, -1.0, 1.0);
}
class SceneManager{
    private:
        std::shared_ptr<Scene> onScene, nextScene;
        GLFWwindow* window;
        void GLinit(int w, int h){
            //glwf init
            if(!glfwInit()){
                exit(-1);
            }
            window = glfwCreateWindow(w, h, "GLgame", nullptr, nullptr);
            if(!window){
                glfwTerminate();
                exit(-1);
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);

            //callback
            glfwSetKeyCallback(window, GLkeyCallback);
            glfwSetWindowSizeCallback(window, resize);

            //gl init
            resize(window, w, h);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    public:
        AudioManager audio;

        SceneManager(int w=1280, int h=960){
            onScene.reset();
            nextScene.reset();
            GLinit(w/2, h/2);
        }
        ~SceneManager(){
            glfwTerminate();
        }
        void getWindowSize(int* width, int *height){
            glfwGetWindowSize(window, width, height);
            (*width) *= 2;
            (*height) *= 2;
        }
        void setScene(Scene* ptr, Scene* nextptr=nullptr){
            onScene.reset(ptr);
            onScene->setManager(this);
            onScene->init();
            keyScene = onScene;
            if( nextptr ){
                setNextScene(nextptr);
            }
        }
        void setNextScene(Scene* ptr){ nextScene.reset(ptr); ptr->setManager(this); }
        void changeScene(){
            if( nextScene ){
                onScene = keyScene = nextScene;
                nextScene.reset();
                onScene->init();
            }
        }
        void GLmainLoop(){
            while(!glfwWindowShouldClose(window)){
                onScene->idle();
                glClearColor(1, 1, 1, 0);
                glClear( GL_COLOR_BUFFER_BIT );

                glEnable( GL_TEXTURE_2D );
                glColor4ub(255, 255, 255, 255);
                onScene->display();

                glfwSwapBuffers(window);
                glfwPollEvents();
                if( nextScene ){ changeScene(); }
            }
        }
};


#endif