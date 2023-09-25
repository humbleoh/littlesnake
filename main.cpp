#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include <unistd.h>

static constexpr int WIN_WIDTH = 800;
static constexpr int WIN_HEIGHT = 600;
static constexpr int REFRESH_RATE_MS = 33;
static constexpr int CONTROL_UPDATE_RATE_MS = 200;

class Snake {
public:
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        MAX_DIR
    };
    
    Snake(int boardXRange, int boardYRange);
    void ChangeDirection(Direction dir);
    void MoveStep();
    void Draw(SDL_Renderer *pRenderer);
private:
    static constexpr uint8_t BLOCK_WIDTH = 50u;
    static constexpr uint8_t BLOCK_HEIGHT = 50u;
    const int BOARD_XRANGE;
    const int BOARD_YRANGE;
    
    using Cell = struct{ int x; int y; };
    std::vector<Cell> body;
    Direction dirHead = Direction::UP;
};

Snake::Snake(int boardXRange, int boardYRange)
    : BOARD_XRANGE { boardXRange / BLOCK_WIDTH }
    , BOARD_YRANGE { boardYRange / BLOCK_HEIGHT }
    , body { 1 }
{
}

void Snake::ChangeDirection(Direction dir)
{
    dirHead = dir;
}

void Snake::MoveStep()
{
    for (size_t i = body.size() - 1; i > 0; --i) {
        body[i] = body[i - 1];
    }
    
    switch (dirHead) {
    case Direction::UP:
        body[0].y -= 1;
        break;
    case Direction::DOWN:
        body[0].y += 1;
        break;
    case Direction::LEFT:
        body[0].x -= 1;
        break;
    case Direction::RIGHT:
        body[0].x += 1;
        break;
    default:
        break;
    }
    
    if (body[0].x < 0) {
        body[0].x = BOARD_XRANGE - 1;
    } else if (body[0].x >= BOARD_XRANGE) {
        body[0].x = 0;
    }
        
    if (body[0].y < 0) {
        body[0].y = BOARD_YRANGE - 1;
    } else if (body[0].y >= BOARD_YRANGE) {
        body[0].y = 0;
    }
}

void Snake::Draw(SDL_Renderer *pRenderer)
{
    for (auto &&c: body) {
        SDL_Rect rect = { c.x * BLOCK_WIDTH, c.y * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT };
        SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(pRenderer, &rect);
        SDL_RenderFillRect(pRenderer, &rect);
    }
}

static void RunEventLoop(SDL_Renderer *pRenderer);
//static void HandleKeyEvent(const SDL_KeyboardEvent &event);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        //std::cerr << "SDL_Init failed：" << SDL_GetError() << std::endl; 
        return -1;
    }
    
    SDL_Window *pWindow = SDL_CreateWindow("SDL Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!pWindow) {
        //std::cerr << "[ERR] SDL_CreateWindow failed:" << SDL_GetError() << std::endl;
        return -1;
    }
    
    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1,
            SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (!pRenderer) {
        //std::cerr << "[ERR] SDL_CreateRenderer failed:" << SDL_GetError() << std::endl;
        return -1;
    }
    //SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);
    //SDL_RenderClear(pRenderer);
    //SDL_RenderPresent(pRenderer);
    //SDL_SetWindowInputFocus(pWindow);

    RunEventLoop(pRenderer);
    
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}

static void RunEventLoop(SDL_Renderer *pRenderer)
{
    Snake snake(WIN_WIDTH, WIN_HEIGHT);
    volatile bool quit = false;    
    while (!quit) {
    SDL_StopTextInput();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                //std::cout << "[INFO] Quit" << std::endl;
                quit = true;
                break;
            case SDL_KEYDOWN:
                //std::cout << "[INFO] Keydown" << std::endl;
                //HandleKeyEvent(event.key);
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    //std::cout << "[INFO] Key-W pressed" << std::endl;
                    snake.ChangeDirection(Snake::Direction::UP);
                    break;
                case SDLK_x:
                    //std::cout << "[INFO] Key-X pressed" << std::endl;
                    snake.ChangeDirection(Snake::Direction::DOWN);
                    break;
                case SDLK_a:
                    //std::cout << "[INFO] Key-A pressed" << std::endl;
                    snake.ChangeDirection(Snake::Direction::LEFT);
                    break;
                case SDLK_d:
                    //std::cout << "[INFO] Key-D pressed" << std::endl;
                    snake.ChangeDirection(Snake::Direction::RIGHT);
                    break;
                default:
                    //snake.ChangeDirection(Snake::Direction::UNCHANGED);
                    break;
                }
                break;
            case SDL_TEXTINPUT:
                std::cout << "[INFO] Key-D pressed" << std::endl;
                break;
            default:
                break;
            }
        }
        
        static int prevTimestamp2 = SDL_GetTicks();
        int now2 = SDL_GetTicks();
        int duration2 = now2 - prevTimestamp2;
        if (duration2 >= CONTROL_UPDATE_RATE_MS) {
            snake.MoveStep();
            prevTimestamp2 = now2;
        }
        
        static int prevTimestamp = SDL_GetTicks();
        int now = SDL_GetTicks();
        int duration = now - prevTimestamp;
        if (duration >= REFRESH_RATE_MS) {
            SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(pRenderer);
            snake.Draw(pRenderer);
            SDL_RenderPresent(pRenderer);
            prevTimestamp = now;
        }
    }
}
#if 0
static void HandleKeyEvent(const SDL_KeyboardEvent &event)
{
    switch (event.keysym.sym) {
    case SDLK_w:
        std::cout << "[INFO] Key-W pressed" << std::endl;
        break;
    case SDLK_x:
        std::cout << "[INFO] Key-X pressed" << std::endl;
        break;
    case SDLK_a:
        std::cout << "[INFO] Key-A pressed" << std::endl;
        break;
    case SDLK_d:
        std::cout << "[INFO] Key-D pressed" << std::endl;
        break;
    }
}
#endif

