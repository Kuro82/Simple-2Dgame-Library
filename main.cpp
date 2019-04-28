#include"GLgame.hpp"

using namespace std;

void loadAudio(SceneManager &game){
    game.audio.loadMP3("Audio/m-art_Anduril.mp3");
    game.audio.loadMP3("Audio/shakin1.mp3");
    game.audio.loadMP3("Audio/jump06.mp3");
}

int main(){
    SceneManager game(1920, 1080);
    loadAudio(game);
    game.setScene(new Opening);
    game.GLmainLoop();

    return 0;
}