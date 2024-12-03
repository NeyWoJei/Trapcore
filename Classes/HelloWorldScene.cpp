#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Config.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics(); // Создаем сцену с физикой
    auto physicsWorld = scene->getPhysicsWorld();
    physicsWorld->setGravity(Vec2(0, -1000)); // Устанавливаем гравитацию

    auto layer = HelloWorld::create(); // Создаем слой HelloWorld
    scene->addChild(layer); // Добавляем слой на сцену

    return scene; // Возвращаем полную сцену
}

bool HelloWorld::init()
{
    if (!Layer::init()) {
        return false;
    }

    level1(); // Добавляем уровень (карта из .tmx)
    player(); // Добавляем игрока
    sizeRes(); // Добавляем дополнительные элементы

    // Запускаем метод update каждую секунду
    schedule([this](float deltaTime) {
        update(deltaTime); // Обновляем координаты в каждом кадре
        }, "update_coordinates_key");

    return true;
}

void HelloWorld::update(float deltaTime)
{
    // Обновляем координаты игрока в каждом кадре
    displayPlayerCoordinates();
}


void HelloWorld::level1()
{
 // Скоро
}



void HelloWorld::player()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Создание игрока
    _player = Player::create();
    if (!_player) {
        CCLOG("Ошибка: не удалось создать игрока!");
        return;
    }

    // Позиционируем игрока над землей
    _player->setPosition(Vec2(visibleSize.width / 2, 700)); // 50 - это высота земли

    // Создание физического тела для игрока
    auto playerBody = PhysicsBody::createBox(_player->getContentSize(), PhysicsMaterial(0.0f, 1.0f, 0.0f)); // Без трения, плотность 1
    playerBody->setDynamic(true);  // Игрок движется, то есть динамичный объект
    playerBody->setGravityEnable(true); // Включаем гравитацию

    // Устанавливаем категории и маски для коллизий
    playerBody->setCategoryBitmask(0x01);  // Игрок
    playerBody->setCollisionBitmask(0x02); // Сталкивается с объектами "0x02"
    playerBody->setContactTestBitmask(0x02); // Проверка контакта с объектами "0x02"

    _player->setPhysicsBody(playerBody); // Привязываем физическое тело к игроку

    this->addChild(_player);
}

void HelloWorld::sizeRes()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Hello, World!", Config::General::FontPath, 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height / 2));
    this->addChild(label, 1);

    auto closeItem = MenuItemImage::create("CloseSelected.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    const auto designWidth = 1280.0f;
    closeItem->setScale(visibleSize.width / designWidth);

    closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width / 2,
        visibleSize.height - closeItem->getContentSize().height / 2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void HelloWorld::onEnter()
{
    Layer::onEnter(); // Вызов родительской реализации
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::displayPlayerCoordinates()
{
    // Получаем текущую позицию игрока
    Vec2 playerPos = _player->getPosition();

    // Создаем строку с координатами игрока
    std::string coordinates = "Player X: " + std::to_string(playerPos.x) + " Y: " + std::to_string(playerPos.y);

    // Создаем текстовый объект для отображения координат
    if (!_coordinatesLabel) {
        // Если метка еще не создана, создаем её
        _coordinatesLabel = Label::createWithTTF(coordinates, Config::General::FontPath, 12);
        _coordinatesLabel->setPosition(Vec2(200, Director::getInstance()->getVisibleSize().height - 30)); // Размещаем в верхнем левом углу
        this->addChild(_coordinatesLabel, 1);  // Добавляем метку на слой
    }
    else {
        // Если метка уже существует, обновляем текст
        _coordinatesLabel->setString(coordinates);
    }
}
