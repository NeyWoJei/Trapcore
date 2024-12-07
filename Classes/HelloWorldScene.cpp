#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Config.h"
#include "NeiPan/Player.h"
#include "NeiPan/Camera.h"

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

bool HelloWorld::init() {
    if (!Layer::init()) {
        return false;
    }

    // Создаем игрока
    auto player = Player::create(this);
    if (!player) {
        return false;
    }

    // Создаем камеру
    auto camera = CameraMain::create();  // Теперь создаем объект CameraMain
    if (!camera) {
        return false;
    }

    this->addChild(camera);  // Добавляем камеру на сцену
    this->addChild(player);   // Добавляем персонажа в слой

    // Загружаем уровень
    level1();
    sizeRes();

    // Скрываем стандартную камеру
    auto defaultCamera = Camera::getDefaultCamera();
    if (defaultCamera) {
        CCLOG("Hiding the default camera");

        // Убираем стандартную камеру из рендеринга
        defaultCamera->setCameraFlag(CameraFlag::USER3);  // Убираем её из рендеринга
    }

    // Устанавливаем игрока в камеру
    camera->setPlayer(player);

    return true;
}

// В классе HelloWorld
bool HelloWorld::level1() {
    // Получаем размеры экрана
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Загружаем текстуру земли
    auto ground = Sprite::create("Level1.png");

    // Проверка на ошибку загрузки текстуры
    if (!ground) {
        CCLOG("Ошибка загрузки текстуры земли");
        return false;
    }

    // Устанавливаем физическое тело для земли
    PhysicsMaterial material(0.0f, 1.0f, 0.0f);  // Материал без трения, упругости и плотности
    Size colliderSize(1920, 64);  // Размеры коллайдера земли
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);  // Создаём прямоугольное физическое тело для земли

    // Проверяем, что физическое тело создано
    if (!physicsBody) {
        CCLOG("Ошибка создания физического тела для земли");
        return false;
    }

    physicsBody->setDynamic(false);  // Земля не будет двигаться, это статический объект
    physicsBody->setGravityEnable(false);  // Отключаем влияние гравитации на землю

    // Устанавливаем позицию земли на экране
    ground->setPosition(Vec2(visibleSize.width / 2, 180));  // Земля по центру экрана по ширине

    // Устанавливаем физическое тело земли
    ground->setPhysicsBody(physicsBody);
    this->addChild(ground);


    return true;  // Успешно завершено
}







// Метод для настройки дополнительных элементов на экране
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

// Метод для закрытия приложения
void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender)
{
    Director::getInstance()->end();
}

