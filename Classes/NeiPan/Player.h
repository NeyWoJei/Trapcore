#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    // Создание и инициализация игрока
    static Player* create();
    bool init() override;

    // Обработка клавиш
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Обновление состояния игрока
    void update(float delta) override;

    // Прыжок
    void jump();


    // Анимация ходьбы
    void playerAnimation();

private:
    cocos2d::Sprite* _playerSprite;

    bool isJumping = false;       // Персонаж в прыжке
    bool canDoubleJump = false;   // Разрешен второй прыжок
    bool _isMovingLeft = false;   // Движение влево
    bool _isMovingRight = false;  // Движение вправо
};

#endif // __PLAYER_H__
