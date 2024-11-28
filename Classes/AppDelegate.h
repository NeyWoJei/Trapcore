#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();  // Конструктор
    virtual ~AppDelegate();  // Деструктор

    virtual bool applicationDidFinishLaunching();  // Инициализация приложения
    virtual void applicationDidEnterBackground();  // Приложение ушло в фон
    virtual void applicationWillEnterForeground();  // Приложение вернулось в активное состояние

    void initGLContextAttrs();  // Настройка атрибутов OpenGL
};


#endif // _APP_DELEGATE_H_

