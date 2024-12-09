#ifndef __TRAPS_H__
#define __TRAPS_H__

#include "cocos2d.h"

USING_NS_CC;

class Traps : public Node {
public:
    Traps();
    virtual ~Traps();
    static Traps* create();
    virtual bool init();

    void spawnBlackHole(float yPosition);
    void update(float delta); // Объявление метода update

    void onEnter();

private:
    std::vector<Sprite*> _blackHoles; // Список ловушек
    float _nextTrapY;                 // Высота для следующей ловушки
};

#endif // __TRAPS_H__
