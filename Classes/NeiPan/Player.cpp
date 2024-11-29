#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

Player* Player::create()
{
    Player* player = new Player();
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init()
{
    if (!Sprite::initWithFile("Player.png"))  // Замените на реальный путь к спрайту
    {
        return false;
    }

    // Подключаем обработку ввода клавиш
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        moveLeft();
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        moveRight();
        break;
    case EventKeyboard::KeyCode::KEY_SPACE:
        jump();
        break;
    default:
        break;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    // Вы можете добавить логику для реакции на отпускание клавиш
}

void Player::moveLeft()
{
    this->setPosition(this->getPositionX() - 10, this->getPositionY());  // Перемещение влево
}

void Player::moveRight()
{
    this->setPosition(this->getPositionX() + 10, this->getPositionY());  // Перемещение вправо
}

void Player::jump()
{
    // Простейший способ "прыжка" - можно добавить логику для того, чтобы тело игрока получило силу по оси Y
    this->getPhysicsBody()->applyImpulse(Vec2(0, 500));  // Импульс вверх
}
