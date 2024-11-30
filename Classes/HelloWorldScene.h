#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NeiPan/Player.h"  // Подключаем заголовок игрока

class HelloWorld : public cocos2d::Layer  // Меняем на Layer, так как мы наследуемся от Layer, а не Scene
{
public:
    static cocos2d::Scene* createScene();  // Указываем возвращаемый тип Scene*
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void onEnter() override;

private:
    Player* _player;  // Указатель на игрока
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
