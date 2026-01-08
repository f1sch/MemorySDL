#include "Game.h"
#include "AssetManager.h"

#include <memory>
#include <random>

constexpr auto TEX_WIDTH = 32;
constexpr auto TEX_HEIGHT = 32;

// TODO: Karten müssen wieder umgedreht werden, wenn es keinen Match gab.

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height)
    : m_window(window), m_renderer(renderer),
    m_windowWidth(width), m_windowHeight(height), m_lastHitCardIdx(-1)
{
}

Game::~Game()
{
}

int Game::Init()
{
    m_assetManager = std::make_unique<AssetManager>(m_renderer);
    m_assetManager->LoadTexture("Back", "assets/Back.png");
    m_assetManager->LoadTexture("Skull", "assets/Skull.png");
    m_assetManager->LoadTexture("Coffin", "assets/Coffin.png");
    m_assetManager->LoadTexture("Candle", "assets/Candle.png");
    m_assetManager->LoadTexture("Dagger", "assets/Dagger.png");
    
    std::vector<std::string> frontCards = { "Skull", "Coffin", "Candle", "Dagger"};

    BuildDeck(frontCards);

	return 0;
}

int Game::Update()
{
    SDL_FRect dst_rect{};
    const Uint64 now = SDL_GetTicks();

    // we'll have some textures move around over a few seconds.
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    // as you can see from this, rendering draws over whatever was drawn before it.
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black, full alpha
    SDL_RenderClear(m_renderer);  // start with a blank canvas.

    // Just draw the static texture a few times. You can think of it like a
    // stamp, there isn't a limit to the number of times you can draw with it.

    // top left
    //dst_rect.x = (100.0f * scale);
    //dst_rect.y = 0.0f;
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);
 
    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
    ApplyGridLayout(2, 4);
    
    // bottom right.
    //dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    //dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(m_renderer);  // put it all on the screen!

    return 0;
}

void Game::HitTest(float x, float y)
{
    SDL_Point p = { static_cast<int>(x), static_cast<int>(y) };
    
    for (size_t i{}; i < m_cards.size(); ++i)
    {
        Card& card = m_cards.at(i);

        // Only HitTest Cards that are facedown
        if (card.state != CardState::FaceDown)
            continue;
        
        // Cancel if no Card was hit
        if (!card.contains(x, y))
            continue;
        
        // Do nothing if the same Card is clicked repeatedly
        if (m_lastHitCardIdx == i)
            return;

        card.state = CardState::FaceUp;
        
        // Current Card is the first Card that was clicked
        if (m_lastHitCardIdx < 0) {
            m_lastHitCardIdx = i;
            return;
        }

        Card& lastCardHit = m_cards.at(m_lastHitCardIdx);

        // Only Cards with the same PairId can be matched
        if (card.pairId == lastCardHit.pairId) {
            SDL_Log("Matched!");
            card.state = CardState::Matched;
            lastCardHit.state = CardState::Matched;
        } else {
            SDL_Log("No Match!");
            card.state = CardState::FaceDown;
            lastCardHit.state = CardState::FaceDown;
        }
        m_lastHitCardIdx = -1;
        return;
    }
}

void Game::ApplyGridLayout(int rows, int columns)
{
    SDL_Texture* back = m_assetManager->GetTexture("Back");
    float margin = 5.f;
    float startX = (m_windowWidth / 2.f) - ((TEX_WIDTH * columns) + (margin * (columns - 1)));
    float startY = (m_windowHeight / 2.f) - ((TEX_HEIGHT * rows) + (margin * (rows - 1)));
    
    for (size_t i{}; i < m_cards.size(); ++i)
    {
        int row = static_cast<int>(i) / columns;
        int col = static_cast<int>(i) % columns;

        SDL_Texture* tex = nullptr;
        if (m_cards.at(i).state == CardState::FaceDown) {
            tex = back;
            //tex = m_assetManager->GetTexture(m_cards.at(i).frontKey);
        } else {
            tex = m_assetManager->GetTexture(m_cards.at(i).frontKey);
        }
    
        if (tex) {
            m_cards.at(i).dst = SDL_FRect{
                startX + col * (TEX_WIDTH + margin),
                startY + row * (TEX_HEIGHT + margin),
                TEX_WIDTH, TEX_HEIGHT
            };
            SDL_RenderTexture(m_renderer, tex, NULL, &m_cards.at(i).dst);
        }
    }
}

// Creates a List of shuffled cards
void Game::BuildDeck(const std::vector<std::string>& frontKeys)
{
    m_cards.clear();
    m_cards.reserve(frontKeys.size() * 2);

    int pairId = 0;
    int uniqueId = 0;
    for (const auto& key : frontKeys)
    {
        Card a; a.uniqueId = uniqueId++; a.pairId = pairId; a.frontKey = key;
        Card b; b.uniqueId = uniqueId++; b.pairId = pairId; b.frontKey = key;
        m_cards.push_back(a);
        m_cards.push_back(b);
        ++pairId;
    }

    std::mt19937 rng{ std::random_device{}() };
    std::shuffle(m_cards.begin(), m_cards.end(), rng);
}
