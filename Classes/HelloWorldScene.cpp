#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "NeiPan/Player.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // Создаем сцену с физическим миром
    auto scene = Scene::createWithPhysics();  // Создаем сцену с физическим миром
    auto layer = HelloWorld::create();  // Создаем слой
    scene->addChild(layer);  // Добавляем слой в сцену
    return scene;
}

bool HelloWorld::init()
{
    // Инициализируем сцену как слой
    if (!Layer::init())
    {
        return false;
    }

    // Получаем размеры экрана
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    // Добавляем текст
    auto label = Label::createWithTTF("Hello, World!", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height / 2));
    this->addChild(label, 1);

    // Создаем кнопку
    auto closeItem = MenuItemImage::create("CloseSelected.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    // Масштабируем кнопку в зависимости от разрешения
    closeItem->setScale(visibleSize.width / 1280.0f);

    // Позиционируем кнопку в правом верхнем углу
    closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width / 2,
        visibleSize.height - closeItem->getContentSize().height / 2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    // Создаем игрока
    _player = Player::create();  // Создаем объект игрока
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));  // Размещаем его по центру экрана

    // Создаем физическое тело для игрока
    auto playerBody = PhysicsBody::createBox(_player->getContentSize());  // Создаем физическое тело для игрока
    playerBody->setDynamic(true);  // Игрок будет динамичным
    playerBody->setGravityEnable(true);  // Включаем гравитацию для игрока
    _player->setPhysicsBody(playerBody);  // Привязываем тело к игроку
    playerBody->setMass(1022);

    this->addChild(_player);

    // Создаем землю (кирпичи)
    auto ground = Sprite::create("Bricks.png");
    ground->setPosition(Vec2(visibleSize.width / 2, 50));

    // Создаем физическое тело для земли
    auto groundBody = PhysicsBody::createBox(ground->getContentSize());
    groundBody->setDynamic(false);
    ground->setPhysicsBody(groundBody);

    this->addChild(ground);

    return true;
}

void HelloWorld::onEnter()
{
    Layer::onEnter();  // Вызов родительского onEnter()

    // Получаем физический мир сцены и устанавливаем гравитацию
    auto physicsWorld = this->getScene()->getPhysicsWorld();  // Получаем физический мир сцены
    if (physicsWorld)
    {
        physicsWorld->setGravity(Vec2(0, -1000));  // Установите сильную гравитацию
    }
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();  // Закрытие приложения
}
