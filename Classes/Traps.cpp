#include "Traps.h"
#include "NeiPan/Player.h"


USING_NS_CC;

// Конструктор
Traps::Traps() {
    _blackHoles.clear();
    _nextTrapY = 1000; // Начальная высота для генерации ловушек
}

// Деструктор
Traps::~Traps() {
    _blackHoles.clear();
}

// Создание экземпляра
Traps* Traps::create() {
    Traps* traps = new Traps();
    if (traps && traps->init()) {
        traps->autorelease();
        return traps;
    }
    CC_SAFE_DELETE(traps);
    return nullptr;
}

// Инициализация
bool Traps::init() {
    if (!Node::init()) {
        return false;
    }

    return true;
}

// Создание ловушки
void Traps::spawnBlackHole(float yPosition) {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Создаем спрайт ловушки с заданным Rect (80x60 пикселей)
    auto blackHole = Sprite::create("blackhole.png", Rect(0, 0, 80, 60));
    if (!blackHole) {
        CCLOG("Ошибка загрузки текстуры ловушки");
        return;
    }

    // Генерация случайной позиции по X
    float x = static_cast<float>(rand() % static_cast<int>(visibleSize.width));

    // Устанавливаем позицию ловушки
    blackHole->setPosition(Vec2(x, yPosition)); // Случайная позиция по X, фиксированная по Y
    blackHole->setScale(1.5f); // Масштабируем ловушку, если нужно

    // Добавляем анимацию (кадры поочередно, без смещения)
    auto animation = Animation::create();
    for (int i = 0; i < 8; ++i) { // 8 кадров (для 8 спрайтов в PNG)
        animation->addSpriteFrameWithTexture(blackHole->getTexture(), Rect(i * 61, 0, 61, 60)); // Размер кадра 61x60
    }
    animation->setDelayPerUnit(0.1f);  // Задержка между кадрами
    animation->setLoops(-1); // Бесконечный цикл

    auto animate = Animate::create(animation);
    blackHole->runAction(animate); // Запуск анимации

    // Добавляем ловушку на сцену
    this->addChild(blackHole, 100);

    // Сохраняем в список
    _blackHoles.push_back(blackHole);

    // Устанавливаем маску камеры
    blackHole->setCameraMask((unsigned short)CameraFlag::USER1, true);
}


void Traps::update(float delta) {
    auto player = dynamic_cast<Player*>(getParent()->getChildByName("Player"));
    if (player) {
        float playerHeight = player->getPositionY();

        // Генерация ловушек только если игрок поднялся выше следующей запланированной ловушки
        if (playerHeight > _nextTrapY) {
            spawnBlackHole(_nextTrapY + 600);  // Новая ловушка каждые 300 единиц высоты
            _nextTrapY += 300;
        }

        // Проверяем столкновения игрока с ловушками
        for (auto it = _blackHoles.begin(); it != _blackHoles.end();) {
            auto blackHole = *it;

            // Проверяем, касается ли игрок ловушки
            if (blackHole->getBoundingBox().intersectsRect(player->getBoundingBox())) {
                // Если столкновение произошло, вызываем fadeOutAndReturnToMenu
                CCLOG("Player hit the trap, fading out and going to the main menu.");
                player->fadeOutAndReturnToMenu();
                return; // Прерываем обновление после столкновения
            }

            // Удаление ловушек, которые вышли за пределы экрана
            if (blackHole->getPositionY() < playerHeight - 600) {
                this->removeChild(blackHole);  // Удаление с экрана
                it = _blackHoles.erase(it);  // Удаление из списка
            }
            else {
                ++it;
            }
        }
    }
}





void Traps::onEnter() {
    Node::onEnter();

    // Инициализация генератора случайных чисел
    srand(time(0));

    // Добавляем обновление уровня (для проверки высоты игрока)
    schedule([this](float deltaTime) {
        update(deltaTime);
        }, "level_update_key");
}
