#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "cocos2d.h"
#include "NeiPan/Player.h"

USING_NS_CC;

class Level1 : public Node
{
public:
    Level1();
    ~Level1();

    // Создание и инициализация уровня
    static Level1* create();
    bool init() override;

    // Переопределение метода onEnter
    void onEnter() override;

    // Метод для создания новой платформы на определенной высоте
    void spawnPlatform(float yPosition);

    // Метод для создания заднего фона на определенной высоте
    void spawnBackground(float yPosition);

    // Метод для создания метки высоты
    void spawnLabel(float yPosition);

    // Метод для обновления логики уровня (проверка высоты игрока)
    void update(float delta);

protected:
    // Вектор для хранения платформ и фонов
    std::vector<cocos2d::Sprite*> _backgrounds; // Хранит спрайты задних фонов
    std::vector<Sprite*> _platforms;           // Хранит спрайты платформ

    // Вектор для хранения меток высоты
    std::vector<cocos2d::Label*> _coordinatesLabels;

    // Переменные для отслеживания высоты
    float _highestPlatformY; // Высота самой верхней платформы
    float _nextPlatformY;    // Высота, на которой создается следующая платформа
    float _nextLabelY;       // Высота, на которой создается следующая метка высоты

private:
    // Ссылка на текущий слой (если необходима)
    Node* _layer = nullptr;

    // Метка высоты (для текущего уровня)
    cocos2d::Label* _coordinatesLabel = nullptr;
};

#endif // __LEVEL1_H__
