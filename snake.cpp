#include "snake.h"

#include <SDL2/SDL.h>

Snake::Snake(std::size_t boardXRange, std::size_t boardYRange)
    : m_COORD_XMAX { static_cast<int>(boardXRange / BLOCK_WIDTH) }
    , m_COORD_YMAX { static_cast<int>(boardYRange / BLOCK_HEIGHT) }
    , m_body { m_COORD_XMAX / 2, m_COORD_YMAX / 2 }
{
}

void Snake::SetStepDirection(Direction dir)
{
    m_curDir = dir;
}

void Snake::MoveStep()
{
    switch (m_curDir) {
    case Direction::UP:
        m_body.y -= 1;
        break;
    case Direction::DOWN:
        m_body.y += 1;
        break;
    case Direction::LEFT:
        m_body.x -= 1;
        break;
    case Direction::RIGHT:
        m_body.x += 1;
        break;
    default:
        break;
    }
    
    if (m_body.x < 0) {
        m_body.x = m_COORD_XMAX - 1;
    } else if (m_body.x >= m_COORD_XMAX) {
        m_body.x = 0;
    }
        
    if (m_body.y < 0) {
        m_body.y = m_COORD_YMAX - 1;
    } else if (m_body.y >= m_COORD_YMAX) {
        m_body.y = 0;
    }
    
    m_curDir = Direction::STATIC;
}

void Snake::Draw(SDL_Renderer *pRenderer)
{
    if (pRenderer) {
        const int x = m_body.x * BLOCK_WIDTH;
        const int y = m_body.y * BLOCK_HEIGHT;
        SDL_Rect rect = { x, y, BLOCK_WIDTH, BLOCK_HEIGHT };
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(pRenderer, &rect);
        SDL_RenderFillRect(pRenderer, &rect);
    }
}
