#include "Scenas/SceneSettings.h"
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
    auto ExitToMenu = ui::Button::create("buttonMenu.png");

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
           });

        this->addChild(ExitToMenu, 1);
    }

    return true;
}
