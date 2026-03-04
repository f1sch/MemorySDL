#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

#include <unordered_map>

class SoundSystem
{
public:
    enum class SoundId { None, Background, CardFlip };

    SoundSystem();
    ~SoundSystem();
    void Init();
    void Update();
    void ShutdownSound();
    void PlaySfxSound(SoundId id);
    void LoadSound(SoundId id, const char* path);


    bool IsInitialised() const;
    bool IsMusicStarted() const;

    void SetMusicStarted(bool start);
private:
    void PlayMusic(SoundId id);
    struct SoundData
    {
        Uint8* wavData;
        Uint32 wavDataLen;
    };

    bool m_initialised;

    SDL_AudioDeviceID m_device;
    SDL_AudioStream* m_musicStream;
    SoundId m_currentMusic;
    bool m_musicStarted;
    bool m_musicPlaying;
    SDL_AudioStream* m_sfxStream;
    SDL_AudioSpec m_deviceSpec;
    
    std::unordered_map<SoundId, SoundData> m_sounds;
};