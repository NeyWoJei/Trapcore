#include "Camera.h"

CameraMain::CameraMain()
    : camera(nullptr), player(nullptr), offsetX(0), offsetY(0) {}

CameraMain::~CameraMain() {}

bool CameraMain::init() {
    if (!Node::init()) {
        return false;
    }

    // Создаём новую камеру
    camera = Camera::create();
    camera->setCameraFlag(CameraFlag::USER1); // Устанавливаем как основную
    camera->setPosition3D(Vec3(0, 0, 500));     // Начальная позиция камеры
    camera->lookAt(Vec3(0, 0, 0));              // Камера смотрит на (0, 0, 0)
    this->addChild(camera);                     // Добавляем камеру в узел `CameraMain`

    // Обновление позиции камеры
    schedule([this](float delta) {
        updateCameraPosition(delta);
    }, "update_camera_key");

    return true;
}


CameraMain* CameraMain::create() {
    CameraMain* ret = new CameraMain();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

void CameraMain::setPlayer(Node* playerNode) {
    player = playerNode;
}

void CameraMain::updateCameraPosition(float delta) {
    if (player) {
        Vec2 playerPos = player->getPosition();  // Получаем позицию игрока
        camera->setPosition3D(Vec3(600, playerPos.y + offsetY, 1000)); 
    }
}
