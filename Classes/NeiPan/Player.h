#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class Player : public Sprite
{
public:
    // Конструктор и деструктор
    static Player* create();
    bool init() override;

    // Метод обновления, вызывается каждый кадр
    void update(float delta) override;

    // Обработчики нажатия и отпускания клавиш
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // Движение влево и вправо
    void moveLeft();
    void moveRight();
    void stopMoving();

    // Прыжок
    void jump();
    void stopJumping();  // Прекращение прыжка

    // Коллизия с землей
    void onGroundCollision(cocos2d::PhysicsContact& contact);

    // Геттеры и сеттеры для состояния
    bool getIsOnGround() const { return isOnGround; }
    void setIsOnGround(bool onGround) { isOnGround = onGround; }

    bool getIsJumping() const { return isJumping; }
    void setIsJumping(bool jumping) { isJumping = jumping; }

private:
    bool isOnGround = false;   // Находится ли персонаж на земле
    bool isJumping = false;    // Прыгает ли персонаж
    bool canDoubleJump = false; // Можно ли выполнить второй прыжок
    float jumpSpeed = 0.0f;    // Скорость прыжка

    bool _isMovingLeft = false;   // Флаг для движения влево
    bool _isMovingRight = false;  // Флаг для движения вправо

    // Константы для скорости и силы
    const float jumpForce = 1000.0f;   // Сила прыжка
    const float moveSpeed = 200.0f;    // Скорость движения по оси X
    const float gravity = 10.0f;       // Ускорение свободного падения (для изменения гравитации)
};

#endif // __PLAYER_H__
