#include "HelloWorldScene.h"
#include "TinoTesla/TinoTeslaMain.h"
#include "Config.h"
#include "SoundsGame.h"
#include "Scenas/SceneSettings.h"
#include "ui/CocosGUI.h" // для использования UI-компонентов
#include "Traps.h"

USING_NS_CC;
using namespace Config::UI;
using namespace Config::General;

bool isSoundOn = true;

Scene* TinoTeslaMain::createScene()
{
    auto scene = Scene::create();
    auto layer = TinoTeslaMain::create();  // Создаем слой с вашей сценой
    scene->addChild(layer);  // Добавляем слой в сцену
    return scene;
}

// Функция для вывода ошибки при загрузке ресурсов
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// Инициализация сцены
bool TinoTeslaMain::init()
{
    if (!Scene::init())  // Используем Scene::init() вместо Layer::init()
    {
        return false;
    }

    SoundGame::getInstance()->playBackgroundMusic("background_music.mp3");

    // Получаем размер экрана
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Создание названия игры
    auto NameGame = Label::createWithTTF(gameName, FontPath, TitleFontSizeGame);
    if (NameGame != nullptr)
    {
        NameGame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.7));
        this->addChild(NameGame, 1);
    }
    else
    {
        problemLoading(FontPath.c_str());
        return false;
    }

    // Создание кнопки PLAY
    auto Button = ui::Button::create(ButtonTexture);
    if (Button != nullptr && Button->getContentSize().width > 0 && Button->getContentSize().height > 0)
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height * 0.3;
        Button->setPosition(Vec2(x, y));
        Button->setScale(ButtonScale);
        Button->setTitleText("PLAY");
        Button->setTitleFontName(FontPath);
        Button->setTitleFontSize(TitleFontSizeLarge);

        Button->addClickEventListener([](Ref* sender)
            {
                auto Level1Scene = HelloWorld::createScene();

                if (Level1Scene != nullptr)
                {
                    // Отключаем звуки
                    AudioEngine::pauseAll();

                    SoundGame::getInstance()->playSound("ButtonSong.mp3", 0.2f);
                    SoundGame::getInstance()->playSound("LevelSong.mp3", 0.2f);
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, Level1Scene)); // Переход с эффектом затухания
                }
                else
                {
                    CCLOG("Ошибка при создании сцены HelloWorld.");
                }
        });
                
    this->addChild(Button, 1);
    }
    else
    {
        problemLoading(ButtonTexture.c_str());
        return false;
    }

    // Создание кнопки SETTINGS
    auto ButtonSettings = ui::Button::create(ButtonTexture);
    if (ButtonSettings != nullptr && ButtonSettings->getContentSize().width > 0 && ButtonSettings->getContentSize().height > 0)
    {
        ButtonSettings->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2));
        ButtonSettings->setTitleText("Settings");
        ButtonSettings->setTitleFontName(FontPath);
        ButtonSettings->setTitleFontSize(TitleFontSizeLarge);
        ButtonSettings->setScale(ButtonScale);

        ButtonSettings->addClickEventListener([](Ref* sender) {
            auto scene = SceneSettings::create(); // Создаем новую сцену
            if (scene != nullptr)
            {
                SoundGame::getInstance()->playSound("ButtonSong.mp3", 0.2f);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene)); // Переход с эффектом затухания
            }
            else
            {
                CCLOG("Ошибка при создании сцены Settings.");
            }
        });

        this->addChild(ButtonSettings, 1);
    }
    else
    {
        problemLoading(ButtonTexture.c_str());
        return false;
    }

    // Создание кнопки EXIT
    auto ButtonExit = ui::Button::create(ButtonTexture);
    if (ButtonExit != nullptr && ButtonExit->getContentSize().width > 0 && ButtonExit->getContentSize().height > 0)
    {
        ButtonExit->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.1));
        ButtonExit->setTitleText("EXIT");
        ButtonExit->setTitleFontName(FontPath);
        ButtonExit->setTitleFontSize(TitleFontSizeLarge);
        ButtonExit->setScale(ButtonScale);

        ButtonExit->addClickEventListener([](Ref* sender) {
            Director::getInstance()->end(); // Выход из игры
            });

        this->addChild(ButtonExit, 1);
    }
    else
    {
        problemLoading(ButtonTexture.c_str());
        return false;
    }

    // Добавление гайда
    auto Guide = Label::createWithTTF("W - jump\nA - left \nD - right", FontPath, 24);
    if (Guide != nullptr)
    {
        Guide->setPosition(Vec2(origin.x + visibleSize.width * 0.1, origin.y + visibleSize.height * 0.9));
        this->addChild(Guide, 1);
    }
    else
    {
        CCLOG("Error: Guide label is nullptr.");
        return false;
    }

    // Добавление фона
    auto background = Sprite::create(wallpaperCastle);
    if (background != nullptr)
    {
        background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        background->setContentSize(visibleSize);
        this->addChild(background, 0);
    }
    else
    {
        CCLOG("Error: Unable to load castle.png");
        return false;
    }

    // Добавление версии игры
    auto Version = Label::createWithTTF(ReadyVersion, FontPath, 36);
    if (Version != nullptr)
    {
        Version->setPosition(Vec2(origin.x + visibleSize.width - Version->getContentSize().width * 2.5, origin.y + Version->getContentSize().height));
        this->addChild(Version, 1);
    }
    else
    {
        problemLoading(FontPath.c_str());
        return false;
    }

    return true;
}
