#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    static Player* create();
    virtual bool init();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void moveLeft();
    void moveRight();
    void jump();

private:
    cocos2d::Sprite* _playerSprite;  // Спрайт игрока
};

#endif // __PLAYER_H__
