#pragma once

#include <SDL3/SDL_audio.h>
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
        Uint8* wav_data;
        Uint32 wav_data_len;
    };
	SDL_AudioStream* m_audio;
    SDL_AudioSpec m_spec;
    std::unordered_map<SoundId, SoundData> m_sounds;
};