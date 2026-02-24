#include "pch.h"
#include "SoundSystem.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>

#include <string>

SoundSystem::SoundSystem()
    : m_audio(nullptr), m_spec{}
{
}

SoundSystem::~SoundSystem()
{
    for (auto& [id, sound] : m_sounds)
    {
        SDL_free(sound.wavData);
    }
    if (m_audio)
        SDL_DestroyAudioStream(m_audio); // TODO: error on app exit
}

void SoundSystem::Init()
{
    //SDL_AudioSpec deviceSpec{};
    m_deviceSpec.format = SDL_AUDIO_F32;
    m_deviceSpec.channels = 2;
    m_deviceSpec.freq = 44100;

    m_audio = SDL_CreateAudioStream(&m_wavSpec, &m_deviceSpec);
    m_audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_deviceSpec, NULL, NULL);
    if (!m_audio) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return;
    }

    SDL_ResumeAudioStreamDevice(m_audio);

    LoadSound(SoundId::Background, "assets/spooky.wav");
    LoadSound(SoundId::CardFlip, "assets/cardFlip.wav");
}

void SoundSystem::PlaySound(SoundId id)
{
    auto& sound = m_sounds[id];

    SDL_ClearAudioStream(m_audio);
    SDL_PutAudioStreamData(m_audio, sound.wavData, sound.wavDataLen);
}

void SoundSystem::LoadSound(SoundId id, const char* path)
{
    const char* basePath = SDL_GetBasePath();
    std::string fullPath = std::string(basePath) + path;
    
    //SDL_AudioSpec wavSpec{};
    Uint8* wavData = nullptr;
    Uint32 wavDataLen = 0;

    if (!SDL_LoadWAV(fullPath.c_str(), &m_wavSpec, &wavData, &wavDataLen)) {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return;
    }

    SDL_Log("WAV: %d Hz %d channels", m_wavSpec.freq, m_wavSpec.channels);
    
    m_sounds[id] = { wavData, wavDataLen };
}

void SoundSystem::LoopSound(SoundId id)
{
    auto& sound = m_sounds[id];
    if (SDL_GetAudioStreamQueued(m_audio) < (int)sound.wavDataLen) {
        SDL_PutAudioStreamData(m_audio, sound.wavData, sound.wavDataLen);
    }
}
