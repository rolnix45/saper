#ifndef S_COMMON_H
#define S_COMMON_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        unsigned int arg_board_w;
        unsigned int arg_board_h;
        unsigned int arg_mine_count;
        bool arbuz;
        bool close_window;
} SContext;

#endif
