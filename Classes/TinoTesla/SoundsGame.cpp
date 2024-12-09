#include "SoundsGame.h"
#include "audio/include/AudioEngine.h"

SoundGame* SoundGame::_instance = nullptr;

SoundGame* SoundGame::getInstance() {
    if (!_instance) {
        _instance = new SoundGame();
    }
    return _instance;
}

SoundGame::SoundGame() : _audioID(-1), _isPlaying(false) {}

SoundGame::~SoundGame() {
    // Если музыка играет, останавливаем её перед уничтожением объекта
    stopBackgroundMusic();
}

void SoundGame::playBackgroundMusic(const std::string& filePath) {
    if (!_isPlaying) {
        _audioID = AudioEngine::play2d(filePath, true);  // Воспроизведение в цикле
        _isPlaying = true;
    }
}
// В классе SoundGame
int SoundGame::getAudioID() const {
    return _audioID;
}

void SoundGame::setIsPlaying(bool isPlaying) {
    _isPlaying = isPlaying;
}

void SoundGame::stopBackgroundMusic() {
    if (_isPlaying) {
        AudioEngine::stop(_audioID);  // Остановить текущую музыку
        _audioID = -1;  // Сбросить идентификатор аудио
        _isPlaying = false;  // Музыка больше не играет
    }
}

void SoundGame::playSound(const std::string& filePath, float volume) {
    int audioID = AudioEngine::play2d(filePath);
    AudioEngine::setVolume(audioID, volume);  // Устанавливаем громкость для эффекта
}

void SoundGame::setVolume(float volume) {
    if (_isPlaying) {
        AudioEngine::setVolume(_audioID, volume);  // Изменить громкость фоновой музыки
    }
}

bool SoundGame::isPlaying() const {
    return _isPlaying;
}
