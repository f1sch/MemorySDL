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
    void PlaySound(SoundId id);
    void LoadSound(SoundId id, const char* path);
    void LoopSound(SoundId id);
private:
    struct SoundData
    {
        Uint8* wavData;
        Uint32 wavDataLen;
    };
	SDL_AudioStream* m_audio;
    SDL_AudioSpec m_spec;
    SDL_AudioSpec m_deviceSpec{};
    SDL_AudioSpec m_wavSpec{};
    std::unordered_map<SoundId, SoundData> m_sounds;
};