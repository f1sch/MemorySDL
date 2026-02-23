#pragma once

#define SDL_MAIN_USE_CALLBACKS 1
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

// SDL3
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

// STL
#include <algorithm>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>