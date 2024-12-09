#ifndef __SOUNDS_GAME_H__
#define __SOUNDS_GAME_H__

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;

class SoundGame {
public:
    static SoundGame* getInstance();
    void playBackgroundMusic(const std::string& filePath);
    void stopBackgroundMusic();
    void setVolume(float volume);
    bool isPlaying() const;
    void playSound(const std::string& filePath, float volume);
    
    int getAudioID() const;
    void setIsPlaying(bool isPlaying);

private:
    SoundGame();
    ~SoundGame();

    static SoundGame* _instance;
    int _audioID;
    bool _isPlaying;
};

#endif // __SOUNDS_GAME_H__
