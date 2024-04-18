#ifndef S_BOARD_H
#define S_BOARD_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <field.h>
#include <common.h>

typedef struct {
        unsigned int width;
        unsigned int height;
        unsigned int mine_count;
        unsigned int mines_left;
        unsigned int known_fields;
        bool game_ended;
} Board;

void load_board_res(void);
void create_board(int w, int h, int mine_count, SContext* ctx);
void restart_board(int w, int h, int mine_count, SContext* ctx);
void end_board(bool boom);
void cleanup_fields(void);
void draw_board(SDL_Renderer* renderer);
void mouse_field_inter(bool rmb);
void count_mines_around(Field* field);

#endif
