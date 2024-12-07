#include "Player.h"
#include "cocos2d.h"
#include "Config.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace Config::Player;

// Функция для создания персонажа
Player* Player::create(HelloWorld* layer)
{
    Player* player = new Player();
    if (player && player->init()) {
        player->_layer = layer;  
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

bool Player::init()
{

    CreatePlayer();
    // Обновление состояния игрока
    this->scheduleUpdate();

    return true;
}


// Создание текстуры персонажа и настройка физики
bool Player::CreatePlayer() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Загружаем текстуру игрока
    auto texture = Director::getInstance()->getTextureCache()->addImage(PlayerTexture);
    if (!texture) {
        return false;  // Если текстура не найдена, выходим
    }

    // Указываем начальный кадр для спрайта
    Rect initialFrame(0, 0, 171, 256);
    if (!Sprite::initWithTexture(texture, initialFrame)) {
        return false;  // Если не удалось создать спрайт, выходим
    }

    // Создаем физическое тело
    PhysicsMaterial material(0.1f, 1.0f, 0.5f);
    Size colliderSize(171, 256);
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);
    physicsBody->setRotationEnable(false);
    physicsBody->setMoment(PHYSICS_INFINITY);
    physicsBody->setMass(1);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);
    physicsBody->setCollisionBitmask(0x01); 
    physicsBody->setContactTestBitmask(0x02);  

    this->setScale(0.2);

    // Позиционируем игрока на экране (над землей)
    this->setPosition(Vec2(visibleSize.width / 2, 700)); // Позиция на экране

    // Привязываем физическое тело к спрайту
    this->setPhysicsBody(physicsBody);

    // Подключаем события клавиатуры
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

// Обработка нажатия клавиш
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        _isMovingLeft = true;
        break;
    case EventKeyboard::KeyCode::KEY_D:
        _isMovingRight = true;
        break;
    default:
        break;
    }
}

// Обработка отпускания клавиш
void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        _isMovingLeft = false;
        break;
    case EventKeyboard::KeyCode::KEY_D:
        _isMovingRight = false;
        break;
    default:
        break;
    }
}

// Обновление состояния игрока
void Player::update(float delta)
{
    displayPlayerCoordinates(); // Метод показа координат

    this->setRotation(0);  // Убираем возможный поворот

    auto physicsBody = getPhysicsBody();

    // Ограничиваем скорость падения
    if (physicsBody->getVelocity().y < MaxFallSpeed) {
        physicsBody->setVelocity(Vec2(physicsBody->getVelocity().x, MaxFallSpeed));
    }

    // Управление движением
    if (_isMovingLeft) {
        physicsBody->setVelocity(Vec2(-PlayerMoveSpeed, physicsBody->getVelocity().y));
        this->setFlippedX(true);  // Переворот спрайта влево
    }
    else if (_isMovingRight) {
        physicsBody->setVelocity(Vec2(PlayerMoveSpeed, physicsBody->getVelocity().y));
        this->setFlippedX(false); // Спрайт в нормальное положение
    }
    else {
        physicsBody->setVelocity(Vec2(0, physicsBody->getVelocity().y));
        this->stopActionByTag(1);  // Останавливаем анимацию, если персонаж стоит
    }
}

// Метод для отображения координат игрока
void Player::displayPlayerCoordinates()
{
    auto physicsBody = getPhysicsBody();
    Vec2 playerPos = this->getPosition();
    float fallSpeed = physicsBody->getVelocity().y;

    // Создаем строку с координатами игрока
    std::string coordinates = "Player X: " + std::to_string(playerPos.x) + " Y: " + std::to_string(playerPos.y);
    std::string velocity = "Player velocity: " + std::to_string(fallSpeed);

    // Создаем текстовый объект для отображения координат
    if (!_coordinatesLabel) {
        // Если метки еще не созданы, создаем их
        _coordinatesLabel = Label::createWithTTF(coordinates, Config::General::FontPath, 12);
        _velocityLabel = Label::createWithTTF(velocity, Config::General::FontPath, 12);

        // Устанавливаем позиции меток
        _coordinatesLabel->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 30)); // координаты
        _velocityLabel->setPosition(Vec2(160, Director::getInstance()->getVisibleSize().height - 45)); // скорость

        // Добавляем метки на слой
        if (_layer) {
            _layer->addChild(_coordinatesLabel, 1);
            _layer->addChild(_velocityLabel, 1);
        }
        else {
            CCLOG("Ошибка: Слой не был передан в Player!");
        }
    }
    else {
        // Если метки уже существуют, обновляем их строки
        _coordinatesLabel->setString(coordinates);
        _velocityLabel->setString(velocity);
    }
}
