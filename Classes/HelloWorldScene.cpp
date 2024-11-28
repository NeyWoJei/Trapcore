#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    // Инициализация сцены
    if (!Scene::init())
    {
        return false;
    }

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // Закрыть игру
    Director::getInstance()->end();
}
