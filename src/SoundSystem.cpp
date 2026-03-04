//#include "pch.h"
#include "SoundSystem.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>

#include <string>

SoundSystem::SoundSystem()
    : m_initialised(false), m_device{},
      m_musicStream(nullptr), m_currentMusic(SoundId::None), m_musicStarted(false), m_musicPlaying(false),
      m_sfxStream(nullptr),
      m_deviceSpec{} {
}

SoundSystem::~SoundSystem()
= default;

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

    //SDL_ResumeAudioDevice(m_device);

    LoadSound(SoundId::Background, "assets/spooky.wav");
    m_currentMusic = SoundId::Background;
    LoadSound(SoundId::CardFlip, "assets/cardFlip.wav");

    m_initialised = true;
}

void SoundSystem::Update()
{
    //if (m_musicPlaying)
    //    return;

    PlayMusic(m_currentMusic);
    m_musicStarted = true;

}

void SoundSystem::ShutdownSound()
{
    if (m_device)
    {
        SDL_PauseAudioDevice(m_device);
        if (m_musicStream)
            SDL_UnbindAudioStream(m_musicStream);
        if (m_sfxStream)
            SDL_UnbindAudioStream(m_sfxStream);
    
    }
    if (m_musicStream)
        SDL_DestroyAudioStream(m_musicStream);
    if (m_sfxStream)
        SDL_DestroyAudioStream(m_sfxStream);
    
    if (m_device)
        SDL_CloseAudioDevice(m_device);
    
    for (const auto& [id, sound] : m_sounds)
    {
        SDL_free(sound.wavData);
    }

    m_initialised = false;
    m_musicStarted = false;
    m_musicPlaying = false;
}

void SoundSystem::PlaySfxSound(const SoundId id)
{
    const auto it = m_sounds.find(id);
    if (it == m_sounds.end()) 
        return;

    // TODO: what should happen if a second card is flipped before the sound of the first card ended?
    SDL_ClearAudioStream(m_sfxStream); 
    SDL_PutAudioStreamData(m_sfxStream, it->second.wavData, static_cast<int>(it->second.wavDataLen));
}

void SoundSystem::PlayMusic(const SoundId id)
{
    //if (m_currentMusic == id && m_musicPlaying)
    //    return;

    // StopMusic();

    if (SDL_GetAudioStreamAvailable(m_musicStream) < 4096)
    {
        if (SDL_PutAudioStreamData(m_musicStream, m_sounds[id].wavData, static_cast<int>(m_sounds[id].wavDataLen)))
            m_musicStarted = true;
    }
}

void SoundSystem::LoadSound(const SoundId id, const char* path)
{
    const char* basePath = SDL_GetBasePath();
    const std::string fullPath = std::string(basePath) + path;

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

    SDL_PutAudioStreamData(convertStream, wavData, static_cast<int>(wavDataLen));
    SDL_FlushAudioStream(convertStream);

    const int convertedSize = SDL_GetAudioStreamAvailable(convertStream);
    const auto convertedData = static_cast<Uint8*>(SDL_malloc(convertedSize));

    SDL_GetAudioStreamData(convertStream, convertedData, convertedSize);

    SDL_DestroyAudioStream(convertStream);
    SDL_free(wavData);

    m_sounds[id] = { convertedData, static_cast<Uint32>(convertedSize)};
}

bool SoundSystem::IsInitialised() const
{
    return m_initialised;
}

bool SoundSystem::IsMusicStarted() const
{
    return m_musicStarted;
}

void SoundSystem::SetMusicStarted(const bool start)
{
    m_musicStarted = start;
}
