#include "pch.h"
#include "SoundSystem.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>

#include <string>

SoundSystem::SoundSystem()
    : m_device{}, 
    m_musicStream(nullptr), m_sfxStream(nullptr), 
    m_deviceSpec{}
{
}

SoundSystem::~SoundSystem()
{
    for (auto& [id, sound] : m_sounds)
    {
        SDL_free(sound.wavData);
    }
    if (m_musicStream)
        SDL_DestroyAudioStream(m_musicStream); // TODO: error on app exit
    if (m_device)
        SDL_CloseAudioDevice(m_device);
}

void SoundSystem::Init()
{
    m_deviceSpec.format = SDL_AUDIO_F32;
    m_deviceSpec.channels = 2;
    m_deviceSpec.freq = 44100;
    
    m_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_deviceSpec);
    if (!m_device) 
    {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return;
    }
    
    m_musicStream = SDL_CreateAudioStream(&m_deviceSpec, &m_deviceSpec);
    if (!m_musicStream) 
    {
        SDL_Log("Couldn't create music stream: %s", SDL_GetError());
        return;
    }
    
    m_sfxStream = SDL_CreateAudioStream(&m_deviceSpec, &m_deviceSpec);
    if (!m_sfxStream) 
    {
        SDL_Log("Couldn't create sfx stream: %s", SDL_GetError());
        return;
    }

    SDL_BindAudioStream(m_device, m_musicStream);
    SDL_BindAudioStream(m_device, m_sfxStream);

    SDL_ResumeAudioDevice(m_device);

    LoadSound(SoundId::Background, "assets/spooky.wav");
    LoadSound(SoundId::CardFlip, "assets/cardFlip.wav");
}

void SoundSystem::PlaySfxSound(SoundId id)
{
    auto it = m_sounds.find(id);
    if (it == m_sounds.end()) 
        return;

    // TODO: what should happen if a second card is flipped before the sound of
    // the first card ended?
    SDL_ClearAudioStream(m_sfxStream); 
    SDL_PutAudioStreamData(m_sfxStream, it->second.wavData, it->second.wavDataLen);
}

void SoundSystem::LoadSound(SoundId id, const char* path)
{
    const char* basePath = SDL_GetBasePath();
    std::string fullPath = std::string(basePath) + path;
    
    SDL_AudioSpec wavSpec{};
    Uint8* wavData = nullptr;
    Uint32 wavDataLen = 0;

    if (!SDL_LoadWAV(fullPath.c_str(), &wavSpec, &wavData, &wavDataLen)) 
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return;
    }

    SDL_Log("WAV: %d Hz %d channels", wavSpec.freq, wavSpec.channels);
    
    SDL_AudioStream* convertStream = SDL_CreateAudioStream(&wavSpec, &m_deviceSpec);
    if (!convertStream) 
    {
        SDL_Log("Couldn't create conversion stream: %s", SDL_GetError());
        SDL_free(wavData);
        return;
    }

    SDL_PutAudioStreamData(convertStream, wavData, wavDataLen);
    SDL_FlushAudioStream(convertStream);

    int convertedSize = SDL_GetAudioStreamAvailable(convertStream);
    Uint8* convertedData = (Uint8*)SDL_malloc(convertedSize);

    SDL_GetAudioStreamData(convertStream, convertedData, convertedSize);

    SDL_DestroyAudioStream(convertStream);
    SDL_free(wavData);

    m_sounds[id] = { convertedData, (Uint32)convertedSize};
}

void SoundSystem::LoopSfxSound(SoundId id)
{
    if (SDL_GetAudioStreamAvailable(m_sfxStream) < 4096) 
    {
        PlaySfxSound(id);
    }
}

void SoundSystem::LoopMusic(SoundId id)
{
    if (SDL_GetAudioStreamAvailable(m_musicStream) < 4096)
    {
        SDL_PutAudioStreamData(m_musicStream, m_sounds[id].wavData, m_sounds[id].wavDataLen);
    }
}
