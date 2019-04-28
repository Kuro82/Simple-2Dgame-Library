# coding: utf-8
import subprocess as sbpro

#read Scenes dirctory
filesInScenes = sbpro.check_output(["ls", "Scenes"]).decode("utf-8").strip().split('\n')
scenelist = []
for i in range(len(filesInScenes)):
    if len(filesInScenes[i].split('.'))==2 and filesInScenes[i].split('.')[1] == "hpp":
        scenelist.append(filesInScenes[i].split('.'))

#make Scene.hpp
with open("Scenes.hpp", mode='w') as header:
    header.write("#ifndef GL_GAME_SCENE\n#define GL_GAME_SCENE\n\n")
    header.write("#include\"GLgameCore.hpp\"\n\n")

    #enum
    header.write("enum SceneType {\n")
    for i in range(len(scenelist)):
        header.write("    " + scenelist[i][0].upper() + "_SCENE = " + str(i) + ",\n")
    header.write("};\n\n")

    #setNextSceneCore() forward declaration & setNextScene()
    header.write("static void setNextSceneCore(SceneType type, SceneManager* manager);\n")
    header.write("void setNextScene(SceneType type, SceneManager* manager){\n    setNextSceneCore(type, manager);\n}\n\n")

    #include"scene.hpp"
    for i in range(len(scenelist)):
        header.write("#include\"Scenes/" + scenelist[i][0] + '.' + scenelist[i][1] + "\"\n")

    #setNextSceneCore()
    header.write("\nstatic void setNextSceneCore(SceneType type, SceneManager* manager){\n    switch(type){\n")
    for i in range(len(scenelist)):
        header.write("        case {}_SCENE:\n            manager->setNextScene(new {}());\n            break;\n".format(scenelist[i][0].upper(), scenelist[i][0]))
    header.write("        default:\n            break;\n")
    header.write("    }\n}\n\n")

    #basic scene
    header.write("//This is basic scene.\n//You can make other scene functions like Basic scene's.\n")
    header.write("class Basic : public Scene{\n    std::vector<GLsprite> spr;\n    public:\n")
    header.write("        ~Basic(){\n            int size=spr.size();\n            for(int i=0;i<size;i++){\n")
    header.write("                spr[i].deleteSprite();\n            }\n        }\n        void idle(){}\n")
    header.write("        void display(){}\n        void key_callback(int key, int scancode, int action, int mods);\n")
    header.write("        void init(){}\n};\n\n")

    header.write("#endif")

#compile
sbpro.call(["c++", "main.cpp", "libs/stb-master/deprecated/stb_image.c", "-framework", "OpenGL", "-lglfw", "-framework", "OpenAL", "-lm", "-std=c++11"])

#check include headers
print("--------------")
for scene in scenelist:
    print(scene[0]+'.'+scene[1])
print("--------------")