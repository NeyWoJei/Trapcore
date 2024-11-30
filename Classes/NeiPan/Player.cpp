#include "Player.h"
#include "cocos2d.h"

USING_NS_CC;

// Функция для создания персонажа
Player* Player::create()
{
    Player* player = new Player();
    if (player && player->init()) {
        player->autorelease();  // Автоматическое управление памятью
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

// Инициализация персонажа
bool Player::init()
{
    if (!Sprite::initWithFile("Player.png")) {
        return false;
    }

    // Создание физического материала с нулевым трением
    PhysicsMaterial material(0.1f, 0.0f, 0.5f);  // (плотность, трение, упругость)
    material.restitution = 0.0f;  // Убираем отскок
    material.friction = 0.0f;  // Убираем трение, чтобы не было скольжения

    // Создание физического тела с использованием этого материала
    auto physicsBody = PhysicsBody::createBox(this->getContentSize(), material, Vec2(0, 0));
    physicsBody->setRotationEnable(false);  // Отключаем вращение
    physicsBody->setMoment(PHYSICS_INFINITY); // Бесконечный момент инерции (не вращается)
    physicsBody->setMass(1);  // Убедитесь, что масса объекта установлена

    this->setPhysicsBody(physicsBody);

    // Обновление состояния игрока каждый кадр
    this->scheduleUpdate();

    // Подключение событий клавиатуры
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Player::update(float delta)
{
    this->setRotation(0);  // Сброс поворота, чтобы персонаж не крутился

    auto physicsBody = getPhysicsBody();

    // Проверка, если на земле (скорость по Y не отрицательная или сила на земле)
    if (physicsBody->getVelocity().y <= 0.0) {
        // Если скорость по Y меньше или равна нулю, значит персонаж на земле или падает
        if (isJumping) {
            isJumping = false;  // Персонаж приземлился, можно прыгать снова
            canDoubleJump = true;  // Разрешаем второй прыжок
        }
    }

    // Устанавливаем скорость по X вручную, чтобы не было скольжения
    if (_isMovingLeft) {
        physicsBody->setVelocity(Vec2(-200, physicsBody->getVelocity().y));  // Двигаем влево
    }
    else if (_isMovingRight) {
        physicsBody->setVelocity(Vec2(200, physicsBody->getVelocity().y));  // Двигаем вправо
    }
    else {
        // Когда ни одна клавиша не нажата, останавливаем движение по X
        physicsBody->setVelocity(Vec2(0, physicsBody->getVelocity().y));
    }
}

// Обработка нажатия клавиш
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        if (!_isMovingLeft) {
            _isMovingLeft = true;
        }
        break;
    case EventKeyboard::KeyCode::KEY_D:
        if (!_isMovingRight) {
            _isMovingRight = true;
        }
        break;
    case EventKeyboard::KeyCode::KEY_W:
        jump();  // Прыжок
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

// Прыжок
void Player::jump()
{
    auto physicsBody = this->getPhysicsBody();

    // Если персонаж не прыгает (на земле) — выполняем обычный прыжок
    if (!isJumping) {
        isJumping = true;  // Отметим, что прыжок начался
        canDoubleJump = true;  // Разрешаем второй прыжок
        if (physicsBody) {
            // Применяем импульс для обычного прыжка
            physicsBody->applyImpulse(Vec2(0, 500000));  // Применяем импульс по вертикали
        }
    }
    // Если персонаж уже в воздухе и не использовал двойной прыжок — применяем его
    else if (canDoubleJump) {
        canDoubleJump = false;  // Запрещаем второй прыжок после его использования
        if (physicsBody) {
            // Применяем импульс для второго прыжка
            physicsBody->applyImpulse(Vec2(0, 500000));  // Применяем импульс для двойного прыжка
        }
    }
}
