#pragma once

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

#include <unordered_map>

class SoundSystem
{
public:
    enum class SoundId { Background, CardFlip };

    SoundSystem();
    ~SoundSystem();
    void Init();
    void PlaySfxSound(SoundId id);
    void LoadSound(SoundId id, const char* path);
    
    void LoopSfxSound(SoundId id);
    void LoopMusic(SoundId id);

private:
    struct SoundData
    {
        Uint8* wavData;
        Uint32 wavDataLen;
    };
	
    SDL_AudioDeviceID m_device;
    SDL_AudioStream* m_musicStream;
    SDL_AudioStream* m_sfxStream;
    SDL_AudioSpec m_deviceSpec;
    
    std::unordered_map<SoundId, SoundData> m_sounds;
};