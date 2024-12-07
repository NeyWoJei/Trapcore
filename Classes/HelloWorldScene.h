#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NeiPan/Player.h"  // Подключаем заголовок игрока
#include "physics/CCPhysicsContact.h"

// Класс HelloWorld, наследуемый от cocos2d::Layer
class HelloWorld : public cocos2d::Layer
{
public:

    // Создание сцены
    static cocos2d::Scene* createScene();

    // Инициализация
    virtual bool init();

    // Обработчик для кнопки закрытия
    void menuCloseCallback(cocos2d::Ref* pSender);

    // Методы для создания элементов сцены
    bool level1();
    void sizeRes();

private:
    // Макрос для создания объекта класса
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__

