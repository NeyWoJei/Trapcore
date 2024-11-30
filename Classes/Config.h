#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {

    // Общие настройки
    namespace General {
        const std::string FontPath = "fonts/UnformitalBlack.ttf";
        const std::string VersionGame = "0.0.6";
        const std::string ReadyVersion = "Version Game " + VersionGame;
    }

    // Настройки кнопок
    namespace UI {
        const std::string ButtonTexture = "buttonMenu.png";
        const float ButtonScale = 0.4f;
        const int TitleFontSizeLarge = 50;
        const int TitleFontSizeGame = 36;
    }

    // Настройки игрока
    namespace Player {

        const std::string PlayerTexture = "Player.png";
        const float PlayerScale = 0.4f;
        const float PlayerImpulse = 85000.0f;
        const float MaxFallSpeed = -1000.0f;
        const float PlayerMoveSpeed = 200.0f;

    }
}

#endif // CONFIG_H
