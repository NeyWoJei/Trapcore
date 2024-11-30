#include "HelloWorldScene.h"
#include "TinoTesla/TinoTeslaMain.h"
#include "Config.h"
#include "Scenas/SceneSettings.h"
#include "ui/CocosGUI.h" //для использования UI-компонентов

USING_NS_CC;
using namespace Config::UI;
using namespace Config::General;

Scene* TinoTeslaMain::createScene()
{
    auto scene = Scene::create();
    auto layer = TinoTeslaMain::create();  // Создаем слой с вашей сценой
    scene->addChild(layer);  // Добавляем слой в сцену
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TinoTeslaMain::init()
{
    if (!Scene::init())  // Используем Layer::init() вместо Scene::init()
    {
        return false;
    }

    //получаем размер экрана
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto NameGame = Label::createWithTTF("Trapcore", FontPath, TitleFontSizeGame);
    if (NameGame == nullptr)
    {
        problemLoading(FontPath.c_str());
    }
    else
    {
        NameGame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8));
        this->addChild(NameGame, 1);
    }

    

    //создаем кнопку PLAY
    auto Button = ui::Button::create(Config::UI::ButtonTexture);

    if (Button == nullptr ||
        Button->getContentSize().width <= 0 ||
        Button->getContentSize().height <= 0)
    {
        problemLoading(Config::UI::ButtonTexture.c_str());
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height * 0.3;
        Button->setPosition(Vec2(x, y));
        Button->setScale(Config::UI::ButtonScale);

        Button->setTitleText("PLAY");
        Button->setTitleFontName(Config::General::FontPath);
        Button->setTitleFontSize(Config::UI::TitleFontSizeLarge);
    }


    Button->addClickEventListener([](Ref* sender) {
        auto scene = HelloWorld::createScene(); // Создаем новую сцену
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene)); // Переход с эффектом затухания
        });

    this->addChild(Button, 1);


    //создаем кнопку SETTINGS
    
    auto ButtonSettings = ui::Button::create(ButtonTexture);

    if (ButtonSettings == nullptr ||
        ButtonSettings->getContentSize().width <= 0 ||
        ButtonSettings->getContentSize().height <= 0)
    {
        problemLoading(ButtonTexture.c_str());
    }
    else
    {
        //устанавливаем позицию кнопки
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height * 0.2;
        ButtonSettings->setPosition(Vec2(x, y));

        //устанавливаем текст
        ButtonSettings->setTitleText("Settings");

        // Устанавливаем шрифт кнопки
        ButtonSettings->setTitleFontName(FontPath);
        ButtonSettings->setTitleFontSize(TitleFontSizeLarge);

        ButtonSettings->setScale(ButtonScale);

        ButtonSettings->addClickEventListener([](Ref* sender) {
            auto scene = SceneSettings::create(); // Создаем новую сцену
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene)); // Переход с эффектом затухания
            });
    }

    this->addChild(ButtonSettings, 1);

    //добавление кнопки EXIT
    auto ButtonExit = ui::Button::create(ButtonTexture);

    if (ButtonExit == nullptr ||
        ButtonExit->getContentSize().width <= 0 ||
        ButtonExit->getContentSize().height <= 0)
    {
        problemLoading("'buttonMenu.png'");
    }
    else
    {
        //устанавливаем позицию кнопки
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height * 0.1;
        ButtonExit->setPosition(Vec2(x, y));

        ButtonExit->setScale(ButtonScale);

        // Устанавливаем шрифт кнопки
        ButtonExit->setTitleFontName(FontPath);
        ButtonExit->setTitleFontSize(TitleFontSizeLarge);
        ButtonExit->setTitleText("EXIT");

        ButtonExit->addClickEventListener([](Ref* sender) {
            Director::getInstance()->end();
            });
    }

    this->addChild(ButtonExit, 1);

    // Добавление названии версии
    auto Version = Label::createWithTTF(ReadyVersion, FontPath, 36);
    if (Version == nullptr)
    {
        problemLoading(FontPath.c_str());
    }
    else
    {
        auto labelSize = Version->getContentSize();
        Version->setPosition(Vec2(origin.x + labelSize.width / 2, origin.y + labelSize.height));
        this->addChild(Version, 1);
    }

    return true;
}
