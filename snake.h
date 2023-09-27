#pragma once

#include <SDL2/SDL.h>

#include <cstddef>
#include <cstdint>

class Snake {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT, STATIC };
    
    Snake(std::size_t boardXRange, std::size_t boardYRange);
    void SetStepDirection(Direction dir);
    void MoveStep();
    void Draw(SDL_Renderer *pRenderer);

private:
    static constexpr std::uint8_t BLOCK_WIDTH = 50u;
    static constexpr std::uint8_t BLOCK_HEIGHT = 50u;
    const int m_COORD_XMAX;
    const int m_COORD_YMAX;
    
    using Cell = struct{ int x; int y; };
    Cell m_body;
    Direction m_curDir { Direction::STATIC };
};
