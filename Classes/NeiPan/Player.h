#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "HelloWorldScene.h"

class HelloWorld;

class Player : public cocos2d::Sprite
{
public:
    // Создание и инициализация игрока
    static Player* create(HelloWorld* layer);
    bool init() override;

    // Инициализация координат
    void displayPlayerCoordinates();

    // Обработка клавиш
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Обновление состояния игрока
    void update(float delta) override;
    bool CreatePlayer();


private:
    HelloWorld* _layer = nullptr;
    Player* _player = nullptr; 

    Sprite* _playerSprite;
    cocos2d::Label* _coordinatesLabel = nullptr; // координаты игрока
    cocos2d::Label* _velocityLabel = nullptr; // скорость падения игрока

    bool _isMovingLeft = false;   // Движение влево
    bool _isMovingRight = false;  // Движение вправо
};

#endif // __PLAYER_H__
