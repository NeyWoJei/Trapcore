#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Config.h"
#include "NeiPan/Player.h"
#include "NeiPan/Camera.h"
#include "Levels/Level1.h"
#include "TinoTesla/TinoTeslaMain.h"
#include "Traps.h"

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

    // Добавление фонового слоя (на задний план)
    createWallpaper(); // z-index = -1

    // Установка сцены
    startNewGame();

    blackHole();
    // Инициализация игрока и камеры
    PlayerAndCameraInit(); // z-index = 30

    // Загрузка уровня
    ground();

    Level(); // z-index = 10

    // Размеры и элементы интерфейса
    sizeRes(); // z-index = 40

    // Инициализация меток (координаты, скорость, прыжок)

    // Запуск обновлений
    this->scheduleUpdate();

    return true;
}


void HelloWorld::update(float dt) {
    // Получаем игрока
    auto player = dynamic_cast<Player*>(getChildByName("Player"));
    if (!player) {
        return; // Если игрок не найден, выходим из функции
    }
  

    // Получаем размеры экрана
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float screenWidth = visibleSize.width;
    float screenHeight = visibleSize.height;

    // Обрабатываем позицию игрока и другие события
    if (player) {
        Vec2 playerPosition = player->getPosition();

        // Обработка выхода за пределы экрана по оси X
        if (playerPosition.x > screenWidth) {
            player->setPositionX(-100);  // Перемещаем игрока влево
        }
        else if (playerPosition.x < -100) {
            player->setPositionX(screenWidth);  // Перемещаем игрока вправо
        }

        // Если игрок опустился ниже камеры, перезагружаем сцену
        if (newCamera && playerPosition.y < newCamera->getPositionY() - (screenHeight / 2) - 70) {
            this->removeAllChildrenWithCleanup(true);
            auto mainMenuScene = TinoTeslaMain::createScene();
            Director::getInstance()->replaceScene(mainMenuScene);
            return;  // После этого можно завершить обновление, так как сцена была заменена
        }

        // Если игрок поднимается выше, обновляем камеру
        if (playerPosition.y > Player::maxY) {
            Player::maxY = playerPosition.y;
            newCamera->setPositionY(playerPosition.y);
        }

        // Обновляем уровень, если он есть
        auto level = dynamic_cast<Level1*>(getChildByName("Level1"));
        if (level) {
            level->update(dt);  // Обновляем уровень
        }
    }
}


void HelloWorld::startNewGame() {

    if (newCamera) {
        newCamera->setPosition(Vec2(0, 0));  // Сброс камеры на начальное положение
    }

    Player::maxY = 0; // Сбрасываем максимальную высоту
}


bool HelloWorld::ground() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto ground = Sprite::create("Level1.png");
    if (!ground) {
        CCLOG("Ошибка загрузки текстуры земли");
        return false;
    }

    PhysicsMaterial material(0.0f, 1.0f, 0.0f); // Материал для физического тела
    Size colliderSize(1920, 1080);
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);

    if (!physicsBody) {
        CCLOG("Ошибка создания физического тела для земли");
        return false;
    }

    physicsBody->setDynamic(false);   // Земля не двигается
    physicsBody->setGravityEnable(false); // Земля не подвержена гравитации

    physicsBody->setCategoryBitmask(Player::GroundCategory); // Категория для земли
    physicsBody->setCollisionBitmask(Player::PlayerCategory); // Столкновения с игроком
    physicsBody->setContactTestBitmask(Player::PlayerCategory); // Проверка на контакт с игроком


    ground->setPhysicsBody(physicsBody);
    ground->setName("Ground");

    ground->setPosition(Vec2(visibleSize.width / 2, -200));
    ground->setCameraMask((unsigned short)CameraFlag::USER1, true); // Привязываем землю к новой камере
    this->addChild(ground);

    return true;
}


void HelloWorld::sizeRes() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Hello, World!", Config::General::FontPath, 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height / 2));
    label->setCameraMask((unsigned short)CameraFlag::USER1, true); // Привязываем текст к новой камере
    this->addChild(label, 100);

    auto closeItem = MenuItemImage::create("CloseSelected.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    const auto designWidth = 1280.0f;
    closeItem->setScale(visibleSize.width / designWidth);
    closeItem->setPosition(Vec2(visibleSize.width - closeItem->getContentSize().width / 2,
        visibleSize.height - closeItem->getContentSize().height / 2));

    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setCameraMask((unsigned short)CameraFlag::USER1, true); // Привязываем меню к новой камере
    this->addChild(menu);
}

void HelloWorld::menuCloseCallback(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}

bool HelloWorld::PlayerAndCameraInit() {
    // Создаем игрока
    auto player = Player::create(this);
    if (!player) {
        return false;
    }
    player->setName("Player");

    // Удаление стандартной камеры
    auto defaultCamera = Camera::getDefaultCamera();
    if (defaultCamera) {
        defaultCamera->removeFromParentAndCleanup(true); // Удаляем стандартную камеру
    }

    // Создаем новую камеру через CameraMain
    newCamera = CameraMain::create(); // Присваиваем глобальную переменную
    if (!newCamera) {
        CCLOG("Ошибка создания камеры из CameraMain");
        return false;
    }

    // Настройка камеры в CameraMain
    newCamera->setPlayer(this);

    // Привязываем камеру и игрока к сцене
    this->addChild(player, 100); // Игрок с более высоким z-order
    this->addChild(newCamera, 50); // Камера с более низким z-order


    player->setCameraMask((unsigned short)CameraFlag::USER1, true);

    return true;
}

bool HelloWorld::Level() {
    auto level = Level1::create();
    if (level) {
        level->setName("Level1"); // Устанавливаем имя для уровня
        this->addChild(level, 1);
        level->setCameraMask((unsigned short)CameraFlag::USER1, true);

        return true;
    }
    return false;
}

bool HelloWorld::blackHole() {
    auto blackHole = Traps::create();
    if (blackHole) {
        blackHole->setName("blackHole"); // Устанавливаем имя для уровня
        this->addChild(blackHole, 100);
        blackHole->setCameraMask((unsigned short)CameraFlag::USER1, true);
        return true;

    }
    return false;

}

bool HelloWorld::createWallpaper() {
    auto wallpaper = Sprite::create("Wallpaper.png");
    if (!wallpaper) {
        CCLOG("Error for wallpaper");
        return false;
    }
    wallpaper->setCameraMask((unsigned short)CameraFlag::USER1, true);
    wallpaper->setPosition(Vec2(600, 700));
    wallpaper->setScale(0.8);

    // Добавляем на задний план
    this->addChild(wallpaper, 0);

    return true;
}

