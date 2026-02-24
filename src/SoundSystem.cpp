#include "pch.h"
#include "SoundSystem.h"
#include <SDL3/SDL_filesystem.h>

SoundSystem::SoundSystem()
    : m_audio(nullptr), m_spec{}
{
}

SoundSystem::~SoundSystem()
{
    for (auto& [id, sound] : m_sounds)
    {
        SDL_free(sound.wav_data);
    }
    SDL_DestroyAudioStream(m_audio);
}

void SoundSystem::Init()
{
    SDL_AudioSpec deviceSpec{};
    deviceSpec.format = SDL_AUDIO_F32;
    deviceSpec.channels = 2;
    deviceSpec.freq = 44100;

    m_audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &deviceSpec, NULL, NULL);
    if (!m_audio) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return;
        //return SDL_APP_FAILURE;
    }

    SDL_ResumeAudioStreamDevice(m_audio);

    LoadSound(SoundId::Background, "assets/spooky.wav");
    LoadSound(SoundId::CardFlip, "assets/cardFlip.wav");

    
    //--------------------------------------------------------------------------
    //char* wav_path = NULL;
    //Uint8* wav_data = NULL;
    //Uint32 wav_data_len = 0;
    //// Load background music
    //SDL_asprintf(&wav_path, "%sassets/spooky.wav", SDL_GetBasePath());
    //if (!SDL_LoadWAV(wav_path, &m_spec, &wav_data, &wav_data_len)) {
    //    SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
    //    //return SDL_APP_FAILURE;
    //}
    //SDL_free(wav_path);
    //
    //m_backgroundAudio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_spec, NULL, NULL);
    //if (!m_backgroundAudio) {
    //    SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    //    //return SDL_APP_FAILURE;
    //}
    //
    //SDL_ResumeAudioStreamDevice(m_backgroundAudio);
    //
    //// card flip
    //wav_path = NULL;
    //wav_data = NULL;
    //wav_data_len = 0;
    //
    //// Load background music
    //SDL_asprintf(&wav_path, "%sassets/cardFlip.wav", SDL_GetBasePath());
    //if (!SDL_LoadWAV(wav_path, &m_spec, &wav_data, &wav_data_len)) {
    //    SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
    //    //return SDL_APP_FAILURE;
    //}
    //SDL_free(wav_path);
    //
    //m_soundeffectAudio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_spec, NULL, NULL);
    //if (!m_soundeffectAudio) {
    //    SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    //    //return SDL_APP_FAILURE;
    //}
    //
    //SDL_ResumeAudioStreamDevice(m_soundeffectAudio);
}

void SoundSystem::PlaySound(SoundId id)
{
    auto& sound = m_sounds[id];

    SDL_ClearAudioStream(m_audio);
    SDL_PutAudioStreamData(m_audio, sound.wav_data, sound.wav_data_len);
}

void SoundSystem::LoadSound(SoundId id, const char* path)
{
    const char* basePath = SDL_GetBasePath();
    std::string fullPath = std::string(basePath) + path;
    
    SDL_AudioSpec wavSpec{};
    Uint8* wav_data = nullptr;
    Uint32 wav_data_len = 0;

    //SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), path);
    //if (!SDL_LoadWAV(wav_path, &m_spec, &wav_data, &wav_data_len)) {
    if (!SDL_LoadWAV(fullPath.c_str(), &wavSpec, &wav_data, &wav_data_len)) {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return;
        //return SDL_APP_FAILURE;
    }

    SDL_Log("WAV: %d Hz %d channels", wavSpec.freq, wavSpec.channels);
    
    m_sounds[id] = { wav_data, wav_data_len };
}

void SoundSystem::LoopSound(SoundId id)
{
    auto& sound = m_sounds[id];
    if (SDL_GetAudioStreamQueued(m_audio) < (int)sound.wav_data_len) {
        SDL_PutAudioStreamData(m_audio, sound.wav_data, sound.wav_data_len);
    }
}
