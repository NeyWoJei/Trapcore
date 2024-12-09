#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NeiPan/Player.h"  
#include "NeiPan/Camera.h" // Убедитесь, что подключен правильный заголовочный файл
#include "physics/CCPhysicsContact.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual void update(float dt) override;

    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);

    bool ground();
    bool PlayerAndCameraInit();
    bool Level();
    bool blackHole();
    void sizeRes();
    void startNewGame();
    void displayPlayerCoordinates();

    bool createWallpaper();
    CameraMain* newCamera; // Объявление переменной камеры
private:

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
