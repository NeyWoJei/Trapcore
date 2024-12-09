#include "Player.h"
#include "cocos2d.h"
#include "Config.h"
#include "TinoTesla/SoundsGame.h"
#include "TinoTesla/TinoTeslaMain.h"
#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;
using namespace Config::Player;

float Player::maxY = 0.0f;

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

bool Player::init() {
    CreatePlayer();

    // Обновление состояния игрока
    this->scheduleUpdate();

    // Обработчик контактов
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Player::onContactBegin, this);

    // Регистрация обработчика с фиксированным приоритетом
    _eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);

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
    Rect initialFrame(0, 0, 40, 61);
    if (!Sprite::initWithTexture(texture, initialFrame)) {
        return false;  // Если не удалось создать спрайт, выходим
    }

    // Создаем физическое тело
    PhysicsMaterial material(0.1f, 1.2f, 0.5f);
    Size colliderSize(40, 61);
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);
    physicsBody->setRotationEnable(false);
    physicsBody->setMoment(PHYSICS_INFINITY);
    physicsBody->setMass(1);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(true);

    physicsBody->setCategoryBitmask(Player::PlayerCategory);
    physicsBody->setCollisionBitmask(Player::GroundCategory);
    physicsBody->setContactTestBitmask(Player::GroundCategory);


    this->setName("Player");

    this->setScale(1);

    // Позиционируем игрока на экране (над землей)
    this->setPosition(Vec2(visibleSize.width / 2, 400)); // Позиция на экране

    // Привязываем физическое тело к спрайту
    this->setPhysicsBody(physicsBody);

    // Подключаем события клавиатуры
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
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
    case EventKeyboard::KeyCode::KEY_W:
        jump();  // Прыжок по клавише W

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
bool Player::isOnGround() {
    auto physicsBody = getPhysicsBody();
    // Проверяем, если игрок находится на земле с помощью небольшой коррекции позиции
    return physicsBody->getVelocity().y == 0 && this->getPositionY() <= 0;
}


// Обновление состояния игрока (например, ограничение скорости падения)
void Player::update(float delta) {
    displayPlayerCoordinates(); // Показываем координаты

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

    // Проверка на землю
    if (isOnGround()) {
        _jumpCount = 0;  // Сбрасываем счетчик прыжков, если игрок на земле
    }
}

// Метод для выполнения прыжка
void Player::jump() {
    if (_jumpCount > 0) { // Проверяем, доступен ли прыжок
        auto physicsBody = getPhysicsBody();

        // Устанавливаем кадр анимации прыжка
        setFrame(1);

        int audioID = AudioEngine::play2d("JumpSound.mp3");
        AudioEngine::setVolume(audioID, 0.5f);

        // Таймер для возврата к начальному кадру
        this->runAction(Sequence::create(
            DelayTime::create(0.2f),
            CallFunc::create([this]() {
                setFrame(0); // Возврат к начальному кадру
                }),
            nullptr
                    ));

        // Применяем силу для прыжка
        physicsBody->setVelocity(cocos2d::Vec2(physicsBody->getVelocity().x, JumpSpeed));

        // Обнуляем счётчик прыжков
        _jumpCount = 0;
    }
}



void Player::displayPlayerCoordinates()
{
    // Локализованные строки для отображения данных (например, количество прыжков)
    std::string jumping = "     " + std::to_string(_jumpCount);

    if (!_coordinatesLabel)
    {
        // Создаем метку, если она еще не создана
        _coordinatesLabel = Label::createWithTTF(jumping, Config::General::FontPath, 12);

        // Устанавливаем позицию метки относительно игрока (например, над головой)
        _coordinatesLabel->setPosition(Vec2(0, 70)); // Отступ 70 пикселей от игрока

        // Устанавливаем камеру для метки, чтобы она следовала за камерами (если используется)
        _coordinatesLabel->setCameraMask((unsigned short)CameraFlag::USER1, true);

        // Добавляем метку как дочерний элемент персонажа (она будет двигаться вместе с ним)
        this->addChild(_coordinatesLabel, 30);
    }
    else
    {
        // Если метка уже существует, обновляем ее строку
        _coordinatesLabel->setString(jumping);
    }
}



// Преобразование std::wstring в std::string (UTF-8)
std::string Player::wstring_to_string(const std::wstring& wstr) {
    std::string str(wstr.begin(), wstr.end());
    return str;
}

void Player::setFrame(int frameIndex) {
    // Размер одного кадра
    const int frameWidth = 40;
    const int frameHeight = 61;

    // Смещение для нужного кадра
    Rect frame(frameWidth * frameIndex, 0, frameWidth, frameHeight);
    this->setTextureRect(frame);
}

bool Player::onContactBegin(PhysicsContact& contact) {
    // Логируем событие контакта
    CCLOG("Contact detected!");

    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    // Проверяем, связано ли одно из тел с игроком
    if (bodyA == this->getPhysicsBody() || bodyB == this->getPhysicsBody()) {
        // Определяем, с чем контактирует игрок (земля или другая плитка)
        if (bodyA != this->getPhysicsBody()) {
            // Если контакт с объектом bodyA, проверяем, это ли земля
            if (bodyA->getCategoryBitmask() == GroundCategory) {
                _jumpCount = 1;
                activateBounceFrame();
                int audioID = AudioEngine::play2d("JumpSound.mp3");

            }
        }
        else if (bodyB != this->getPhysicsBody()) {
            // Если контакт с объектом bodyB, проверяем, это ли земля
            if (bodyB->getCategoryBitmask() == GroundCategory) {
                _jumpCount = 1;
                activateBounceFrame();
                int audioID = AudioEngine::play2d("JumpSound.mp3");

            }
        }
    }

    return true;
}

void Player::activateBounceFrame() {
    // Устанавливаем второй кадр
    setFrame(1);

    // Таймер для возврата к начальному кадру
    this->runAction(Sequence::create(
        DelayTime::create(0.2f), // Настраиваем время отображения кадра
        CallFunc::create([this]() {
            setFrame(0); // Возврат к начальному кадру
            }),
        nullptr
                ));
}

void Player::fadeOutAndReturnToMenu() {
    if (this->isRunning()) {
        // Прежде чем выполнять анимацию, останавливаем музыку уровня
        stopLevelMusic();  // Остановим музыку уровня

        // Сама анимация "гашения" персонажа
        auto fadeOut = FadeOut::create(1.0f);

        this->runAction(Sequence::create(
            fadeOut,
            CallFunc::create([this]() {
                CCLOG("Transitioning to Main Menu");

                // Переход в главное меню
                auto menuScene = TinoTeslaMain::createScene();
                if (menuScene != nullptr) {
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene));

                    // Включаем музыку главного меню
                    CCLOG("Starting background music for the main menu...");
                    SoundGame::getInstance()->playBackgroundMusic("background_music.mp3");
                }
                else {
                    CCLOG("Error: Main menu scene is nullptr.");
                }
                }),
            nullptr
                    ));
    }
    else {
        CCLOG("Player is not running, cannot perform fadeOut action.");
    }
}



// В классе Player
void Player::stopLevelMusic() {
    // Получаем экземпляр SoundGame
    SoundGame* soundGame = SoundGame::getInstance();

    // Проверяем, если музыка уровня играет, остановим её
    if (soundGame->isPlaying()) {
        CCLOG("Stopping level music...");
        AudioEngine::stop(soundGame->getAudioID());  // Прямо останавливаем музыку по её ID
        soundGame->setIsPlaying(false); // Обновляем флаг, что музыка больше не играет
    }
}
