#include "Scenas/SceneSettings.h"
#include "TinoTesla/SoundsGame.h"
#include "TinoTesla/TinoTeslaMain.h"  // Подключаем заголовочный файл HelloWorld для перехода на первую сцену
#include "ui/CocosGUI.h"
#include "Config.h"


USING_NS_CC;

using namespace Config::UI;
using namespace Config::General;

Scene* SceneSettings::createScene() {
    return SceneSettings::create(); // Создаем и возвращаем сцену SceneSettings
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

SceneSettings::SceneSettings() {}
SceneSettings::~SceneSettings() {}

bool SceneSettings::init() {
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //добавление текста спец шрифта
    auto ExitToMenu = ui::Button::create(ButtonTexture);

    if (ExitToMenu == nullptr ||
        ExitToMenu->getContentSize().width <= 0 ||
        ExitToMenu->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width * 0.1;
        float y = origin.y + visibleSize.height * 0.8;
        ExitToMenu->setPosition(Vec2(x, y));
        ExitToMenu->setScale(0.3f);

        //устанавливаем текст

        ExitToMenu->setTitleText("Exit to menu");

        // Устанавливаем шрифт кнопки
        ExitToMenu->setTitleFontName(FontPath);
        ExitToMenu->setTitleFontSize(TitleFontSizeLarge);

        ExitToMenu->addClickEventListener([](Ref* sender) {
            auto scene = TinoTeslaMain::createScene(); // Создаем новую сцену
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene)); // Переход с эффектом затухания

            SoundGame::getInstance()->playSound("ButtonSong.mp3", 0.2f);
           });

        this->addChild(ExitToMenu, 1);
    }

    auto SetVolume = Label::createWithTTF("VOLUME", FontPath, 24);
    if (SetVolume) {
        // Установка позиции текста в центре экрана
        SetVolume->setPosition(Vec2(origin.x + visibleSize.width * 0.4,
            origin.y + visibleSize.height * 0.3));

        // Добавление текста на сцену
        this->addChild(SetVolume, 1);
    }

    auto PercentLabel = Label::createWithTTF("50", FontPath, 16);
    if (PercentLabel) {
        // Установка позиции текста в центре экрана
        PercentLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height * 0.34));

        // Добавление текста на сцену
        this->addChild(PercentLabel, 1);
    }

    auto background2 = Sprite::create(wallpaperCastle);

    if (background2 == nullptr) {
        CCLOG("Error: Unable to load background.png");
        return false;
    }

    background2->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));
    background2->setContentSize(visibleSize);
    this->addChild(background2, 0);

    auto slider = ui::Slider::create();

        // Устанавливаем изображения для ползунка
    slider->loadBarTexture(sliderTrack);          // Полоса
    slider->loadSlidBallTextures(sliderThumb);    // Ползунок
    slider->loadProgressBarTexture(sliderProgress); // Прогресс

        // Позиция и размер
    slider->setPosition(Vec2(visibleSize.width / 2, origin.y + visibleSize.height * 0.3));
    slider->setPercent(1); 

    slider->addEventListener([=](Ref* sender, ui::Slider::EventType type) {
        if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
            // Обновляем текст метки с текущим значением процента
            int percent = slider->getPercent();
            PercentLabel->setString(std::to_string(percent));

            auto slider = dynamic_cast<ui::Slider*>(sender);
            float volume = slider->getPercent() / 100.0f;  // Преобразуем процент в диапазон [0.0, 1.0]
            SoundGame::getInstance()->setVolume(volume);
        }
        });

        // Добавляем ползунок на слой
    this->addChild(slider);
    
    return true;
}
