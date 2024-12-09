#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "Physics/CCPhysicsContact.h"

class HelloWorld;

class Player : public cocos2d::Sprite
{
public:
    // Создание и инициализация игрока
    static Player* create(HelloWorld* layer);
    bool init() override;

    static float maxY;

    // Инициализация координат
    void displayPlayerCoordinates();

    // Обработка клавиш
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // Обновление состояния игрока
    void update(float delta) override;
    bool CreatePlayer();

    // Проверка, находится ли игрок на земле
    bool isOnGround();

    // Сбросить количество прыжков, когда игрок коснулся земли
    void resetJumpCount() {
        _jumpCount = 0;
    }

    // Метод для выполнения прыжка
    void jump(); // Только объявление
    void setFrame(int frameIndex);
    void activateBounceFrame();
    void fadeOutAndReturnToMenu();
    void stopLevelMusic();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    // Проверка, может ли игрок прыгнуть
    bool canJump() const {
        return _jumpCount < _maxJumps;
    }

    // Убираем 'extern' и оставляем только 'static const'
    static const int PlayerCategory = 0x01; // Пример категории для игрока
    static const int GroundCategory = 0x02; // Пример категории для земли

private:
    std::string wstring_to_string(const std::wstring& wstr);

    HelloWorld* _layer = nullptr;
    Player* _player = nullptr;

    Sprite* _playerSprite;
    cocos2d::Label* _coordinatesLabel = nullptr; // координаты игрока
    cocos2d::Label* _velocityLabel = nullptr; // скорость падения игрока
    cocos2d::Label* _isJumpingLabel = nullptr; // скорость падения игрока

    bool _isMovingLeft = false;   // Движение влево
    bool _isMovingRight = false;  // Движение вправо
    bool isJumping = false; 
    bool canDoubleJump = true; 
    bool isBouncing = false;

    int _currentFrame = 0; // Текущий кадр
    Sprite* _sprite = nullptr;

    const float JumpSpeed = 1050.0f;
    int _jumpCount = 1;      // Счетчик прыжков
    const int _maxJumps = 1; // Максимальное количество прыжков

};

#endif // __PLAYER_H__
