#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "NeiPan/Player.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // Создаем сцену с физикой
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

    // Получаем родительский объект сцены, чтобы получить доступ к физическому миру
    auto scene = dynamic_cast<Scene*>(this->getParent());  // Получаем родительскую сцену
    if (scene)
    {
        auto physicsWorld = scene->getPhysicsWorld();  // Получаем физический мир сцены
        physicsWorld->setGravity(Vec2(0, -1000));  // Устанавливаем гравитацию по оси Y
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

    this->addChild(_player);

    // Создаем землю (кирпичи)
    auto ground = Sprite::create("Bricks.png");  // Убедитесь, что файл существует
    ground->setPosition(Vec2(visibleSize.width / 2, 50));  // Позиционируем землю внизу экрана

    // Создаем физическое тело для земли
    auto groundBody = PhysicsBody::createBox(ground->getContentSize());
    groundBody->setDynamic(false);  // Земля не двигается
    ground->setPhysicsBody(groundBody);  // Привязываем физическое тело к земле

    this->addChild(ground);

    return true;
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();  // Закрытие приложения
}
