#ifndef CAMERAMAIN_H
#define CAMERAMAIN_H

#include "cocos2d.h"
using namespace cocos2d;

class CameraMain : public Node {
public:
    CameraMain();
    ~CameraMain();
    static CameraMain* CameraMain::create();

    bool init();

    
    void setPlayer(Node* playerNode);  // Метод для установки игрока
    void updateCameraPosition(float delta);  // Метод для обновления позиции камеры

    // Геттер для камеры, если нужно работать с камерой напрямую
    Camera* getCamera() { return camera; }

private:
    Camera* camera;   // Камера
    Node* player;     // Узел игрока
    float offsetX;    // Смещение камеры по оси X
    float offsetY;    // Смещение камеры по оси Y
};

#endif // CAMERAMAIN_H
