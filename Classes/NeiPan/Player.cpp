#include "Player.h"
#include "cocos2d.h"
#include "Config.h"

USING_NS_CC;
using namespace Config::Player;

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

    // Предположим, что начальный кадр (0 кадр) находится в верхнем левом углу
    auto texture = Director::getInstance()->getTextureCache()->addImage(PlayerTexture);

    // Указываем начальный кадр: прямоугольник, например, 32x64 пикселей
    Rect initialFrame(0, 0, 60, 128);

    if (!Sprite::initWithTexture(texture, initialFrame)) {
        return false;
    }

    // Создание физического тела
    PhysicsMaterial material(0.1f, 0.0f, 0.5f);
    Size colliderSize(60, 128);
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);
    physicsBody->setRotationEnable(false);
    physicsBody->setMoment(PHYSICS_INFINITY);
    physicsBody->setMass(1);

    this->setScale(PlayerScale);
    this->setPhysicsBody(physicsBody);

    // Подключение событий клавиатуры
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Обновление состояния игрока
    this->scheduleUpdate();

    return true;
}



// Обработка нажатия клавиш
void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_A:
        _isMovingLeft = true;
        playerAnimation();  // Запускаем анимацию бега влево
        break;
    case EventKeyboard::KeyCode::KEY_D:
        _isMovingRight = true;
        playerAnimation();  // Запускаем анимацию бега вправо
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

// Обновление состояния игрока
void Player::update(float delta)
{
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

    // Проверка приземления
    if (physicsBody->getVelocity().y == 0 && isJumping) {
        isJumping = false;
        canDoubleJump = true;

        // Возвращаемся к первому кадру при приземлении
        auto texture = Director::getInstance()->getTextureCache()->addImage(PlayerTexture);
        if (texture) {
            Rect idleFrameRect(0, 0, 60, 128);  // Первый кадр
            auto idleFrame = SpriteFrame::createWithTexture(texture, idleFrameRect);
            if (idleFrame) {
                this->setSpriteFrame(idleFrame);  // Устанавливаем кадр для стояния
            }
        }
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
            physicsBody->applyImpulse(Vec2(0, PlayerImpulse));  // Применяем импульс для прыжка
        }

        // Смена кадра для прыжка
        auto texture = Director::getInstance()->getTextureCache()->addImage(PlayerTexture);
        if (texture) {
            Rect jumpFrameRect(4 * 61, 0, 61, 128);  // Кадр прыжка — пятый (320 - 64)
            auto jumpFrame = SpriteFrame::createWithTexture(texture, jumpFrameRect);
            if (jumpFrame) {
                this->setSpriteFrame(jumpFrame);  // Устанавливаем кадр прыжка
            }
        }
    }
    // Если персонаж уже в воздухе и не использовал двойной прыжок — применяем его
    else if (canDoubleJump) {
        canDoubleJump = false;  // Запрещаем второй прыжок после его использования

        if (physicsBody) {
            physicsBody->applyImpulse(Vec2(0, PlayerImpulse));  // Применяем импульс для второго прыжка
        }
    }
}


void Player::playerAnimation()
{
    // Останавливаем текущую анимацию, если она существует
    if (this->getActionByTag(1)) {
        this->stopActionByTag(1);
    }

    // Загружаем текстуру для анимации
    auto texture = Director::getInstance()->getTextureCache()->addImage(PlayerTexture);
    if (!texture) {
        CCLOG("Ошибка: текстура Player.png не найдена.");
        return;
    }

    // Вектор для хранения кадров анимации
    Vector<SpriteFrame*> frames;

    // Размеры кадров
    const int frameWidth = 61;
    const int frameHeight = 128;

    // Анимация бега (первые 4 кадра)
    for (int i = 0; i < 4; ++i) {
        int x = i * frameWidth;  // Смещение по оси X для каждого кадра
        Rect frameRect(x, 0, frameWidth, frameHeight);  // Координаты кадра
        auto frame = SpriteFrame::createWithTexture(texture, frameRect);
        if (frame) {
            frames.pushBack(frame);  // Добавляем кадр в вектор
        }
    }

    // Создаем анимацию с интервалом 0.1 сек
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(animation);

    // Создаем повторяющуюся анимацию
    auto repeat = RepeatForever::create(animate);
    repeat->setTag(1);  // Устанавливаем тэг для управления анимацией

    // Запускаем анимацию
    this->runAction(repeat);
}

