#ifndef __SCENE_SETTINGS_H__
#define __SCENE_SETTINGS_H__

#include "cocos2d.h"

class SceneSettings : public cocos2d::Scene
{
public:
    SceneSettings();
    ~SceneSettings();

    static Scene* createScene(); // Статический метод для создания сцены
    virtual bool init(); // Инициализация сцены

    CREATE_FUNC(SceneSettings); // Макрос для создания сцены
};

#endif // __SCENE__H__