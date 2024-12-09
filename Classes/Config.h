﻿#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {

    // Общие настройки
    namespace General {
        const std::string FontPath = "fonts/retro-land-mayhem.ttf";
        const std::string VersionGame = "0.2.0";
        const std::string ReadyVersion = "Version Game " + VersionGame;
        const std::string gameName = "Trapjump";
        
    }

    // Настройки кнопок
    namespace UI {
        const std::string ButtonTexture = "buttonMenu.png";
        const float ButtonScale = 0.4f;
        const int TitleFontSizeLarge = 50;
        const int TitleFontSizeGame = 36;

        // Ползунок в настройках
        const std::string sliderTrack = "sliderTrack.png";
        const std::string sliderThumb = "sliderThumb.png";
        const std::string sliderProgress = "sliderProgress.png";

        // Изображение заднего фона
        const std::string wallpaperCastle = "castle.png";
    }

    // Настройки игрока
    namespace Player {

        const std::string PlayerTexture = "Amogus.png";
        const float PlayerScale = 0.25f;
        const float MaxFallSpeed = -1000.0f;
        const float PlayerMoveSpeed = 600.0f;
        const float PlayerImpulse = 1050.0f;

    }
}

#endif // CONFIG_H 
