#include "HelloWorldScene.h"
#include "TinoTesla/TinoTeslaMain.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics(); // Создаем сцену с физикой
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1000)); // Устанавливаем гравитацию

    auto layer = HelloWorld::create(); // Создаем слой HelloWorld
    scene->addChild(layer); // Добавляем слой на сцену

    return scene; // Возвращаем полную сцену
}



bool HelloWorld::init()
{
    if (!Layer::init())
    {
        return false;
    }

    bricks();
    player();
    sizeRes();

    return true;
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::player()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    _player = Player::create();
    if (!_player) {
        CCLOG("Ошибка: не удалось создать игрока!");
        return;
    }
    _player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    auto playerBody = PhysicsBody::createBox(_player->getContentSize());
    playerBody->setDynamic(true);
    playerBody->setGravityEnable(true);
    _player->setPhysicsBody(playerBody);

    this->addChild(_player);
}

void HelloWorld::bricks()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto ground = Sprite::create("Bricks.png");
    ground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));

    auto groundBody = PhysicsBody::createBox(ground->getContentSize());
    groundBody->setDynamic(false);
    ground->setPhysicsBody(groundBody);

    this->addChild(ground);
}

void HelloWorld::sizeRes()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Hello, World!", "fonts/Marker Felt.ttf", 24);
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

