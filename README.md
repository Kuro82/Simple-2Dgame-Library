# Simple-2Dgame-Library
## 概要
このライブラリは、複数のライブラリをラッピングして作られたc++用2Dゲームライブラリです。  
機能としては、複数の形式の画像ファイルの読み込みと描画、mp3ファイルの再生がおこなえます。  
このライブラリを扱うにはOpenGLにglfw、及びOpenALを扱えるようにしておいてください。

macOS (Mojave 10.14.4) 環境でのみ動作確認をしていますが、クロスプラットフォームライブラリのみを使用しているため、Windows環境などでも利用できると思います。  
ただしその際には、glfwやOpenALが扱えるようにコンパイル用スクリプト (compile.sh、compile.py) の一部を変更してください。

## 権利
基本的にpublic domainです。  
ただし利用ライブラリの部分に関しては各自のライセンスに従ってください。  
またサンプルとして入れてあるmp3ファイルに関しては、著作権が存在します[*](#mp3)。  
画像は全てpublic domainです。

## 内訳
### GLgame.hpp
基本ライブラリです。main.cppでincludeしてください。

### GLgameCore.hpp
基本ライブラリで扱える機能の定義部です。  
includeは不要です。  

### Scenes
ゲームシーンクラスを定義したヘッダーファイルを入れるディレクトリです。  
ここに入れたヘッダーはScene.hpp内のchangeScene関数に自動的に対応します (hppのみ対応) 。  
但し、Scene.hpp最下部にあるBasicクラスに準じた形式にしてください。

### compile.sh
このライブラリを利用したmain.cppをコンパイルします。  
警告はたくさん出ます。

### compile.py
基本的にcompile.shと同じです。  
但しScenes内のヘッダーファイルを読み込んで自動的に、Scene.hppを更新します。  
新たなシーンクラスを作成した時には必ず、こちらを一度利用してください。  

## 利用ライブラリ一覧
 * [OpenGL](https://www.opengl.org/)  
 * [OpenAL](https://www.openal.org/)  
 * [glfw](https://www.glfw.org/)  
 * [stb](https://github.com/nothings/stb)  
 * [dr_libs](https://github.com/mackron/dr_libs)  
 
## <a id="mp3">サンプル用mp3ファイル
  * [m-art_Anduril.mp3](http://mart.kitunebi.com/)
  * [jump06.mp3](https://taira-komori.jpn.org/index.html)
  * [shakin1.mp3](https://soundeffect-lab.info/)
