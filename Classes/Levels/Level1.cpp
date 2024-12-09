#include "Level1.h"
#include "cocos2d.h"
#include "NeiPan/Player.h"
#include "NeiPan/Camera.h"
#include "Config.h"

USING_NS_CC;

// Конструктор
Level1::Level1() {
    _platforms.clear();
    _backgrounds.clear();
    _coordinatesLabels.clear(); // Очистка списка меток высоты
    _highestPlatformY = 0; // Изначально верхней платформы нет
    _nextPlatformY = 700;  // Первая платформа будет появляться после того, как игрок поднимется выше Y = 700
    _nextLabelY = 1000;    // Первая метка будет на высоте 1000
}

// Деструктор
Level1::~Level1() {
    _platforms.clear();
    _backgrounds.clear();
    _coordinatesLabels.clear(); // Удаление всех меток высоты
}

Level1* Level1::create() {
    Level1* level = new Level1();
    if (level && level->init()) {
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}

bool Level1::init() {
    if (!Node::init()) {
        return false;
    }

    // Создаем начальный фон
    spawnBackground(1500);   // Следующий фон выше первого


    spawnLabel(1000);

    // Создаем начальные платформы
    spawnPlatform(700);  // Первая платформа на фиксированной высоте
    spawnPlatform(1200); // Вторая платформа выше первой
    spawnPlatform(1500); // Третья платформа выше второй

    return true;
}

// Метод для создания новой платформы
void Level1::spawnPlatform(float yPosition) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Создаем новую платформу
    auto platform = Sprite::create("Platform.png");
    if (!platform) {
        CCLOG("Ошибка загрузки текстуры платформы");
        return;
    }

    // Генерируем случайную позицию по X
    float x = rand() % int(visibleSize.width);
    platform->setPosition(Vec2(x, yPosition));

    // Создаем физическое тело
    Size colliderSize(50, 25);
    PhysicsMaterial material(0.0f, 1.0f, 0.0f);
    auto physicsBody = PhysicsBody::createBox(colliderSize, material);
    physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);

    physicsBody->setCategoryBitmask(Player::GroundCategory); // Категория для земли
    physicsBody->setCollisionBitmask(Player::PlayerCategory); // Столкновения с игроком
    physicsBody->setContactTestBitmask(Player::PlayerCategory); // Проверка на контакт с игроком

    platform->setPhysicsBody(physicsBody);

    // Добавляем платформу на сцену
    this->addChild(platform, 1);

    _platforms.push_back(platform);
    platform->setCameraMask((unsigned short)CameraFlag::USER1, true);

    // Обновляем высоту самой верхней платформы
    _highestPlatformY = std::max(_highestPlatformY, yPosition);
}

// Метод для создания нового заднего фона
void Level1::spawnBackground(float yPosition) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto background = Sprite::create("Wallpaper2.png");
    if (!background) {
        CCLOG("Ошибка загрузки текстуры фона");
        return;
    }

    // Устанавливаем размер и позицию
    background->setContentSize(Size(1920, 1080));
    background->setPosition(Vec2(visibleSize.width / 2, yPosition));
    background->setScale(0.8);



    // Добавляем фон на сцену
    this->addChild(background, 0); // Z-уровень 0 для размещения позади платформ

    _backgrounds.push_back(background);

    background->setCameraMask((unsigned short)CameraFlag::USER1, true);
}

// Метод для создания метки высоты
void Level1::spawnLabel(float yPosition) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    std::string labelText = "Height " + std::to_string(static_cast<int>(yPosition)) + " meters";

    auto label = Label::createWithTTF(labelText, Config::General::FontPath, 24);
    if (!label) {
        CCLOG("Ошибка загрузки шрифта для метки высоты");
        return;
    }
    label->setPosition(Vec2(visibleSize.width / 2, yPosition));

    // Добавляем текст с Z-уровнем выше остальных объектов
    this->addChild(label, 100);

    // Делаем метку первой в списке детей слоя
    this->reorderChild(label, 0);

    _coordinatesLabels.push_back(label);

    label->setCameraMask((unsigned short)CameraFlag::USER1, true);
}


// Метод для проверки, когда следует создать новые объекты
void Level1::update(float delta) {
    auto player = dynamic_cast<Player*>(getParent()->getChildByName("Player"));
    if (player) {
        float playerHeight = player->getPositionY();

        // Генерация платформ
        if (playerHeight > _nextPlatformY) {
            spawnPlatform(_nextPlatformY + 650);
            _nextPlatformY += 400;
        }

        // Генерация заднего фона
        float highestBackgroundY = _backgrounds.empty() ? 0 : _backgrounds.back()->getPositionY();
        if (playerHeight > highestBackgroundY - 540) {
            spawnBackground(highestBackgroundY + 900);
        }

        // Генерация меток высоты
        if (playerHeight > _nextLabelY) {
            spawnLabel(_nextLabelY + 1000);
            _nextLabelY += 1000;
        }
    }
}


void Level1::onEnter() {
    Node::onEnter();

    srand(time(0)); 

    // Добавляем обновление уровня (для проверки высоты игрока)
    schedule([this](float deltaTime) {
        update(deltaTime);
        }, "level_update_key");
}
