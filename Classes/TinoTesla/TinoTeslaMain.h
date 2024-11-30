#ifndef __TINOTESLA_MAIN_H__
#define __TINOTESLA_MAIN_H__

#include "cocos2d.h"

class TinoTeslaMain : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();

private:
    CREATE_FUNC(TinoTeslaMain);
};

#endif // __TINOTESLA_MAIN_H__
