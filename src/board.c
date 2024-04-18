#include <board.h>
#include <field.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <input.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

Field* fields[40][40];
Mix_Chunk* sfx_bum = NULL;
Mix_Chunk* sfx_win = NULL;
TTF_Font* font = NULL ;
Board* current_board;

void
load_board_res(void) {
        sfx_bum = Mix_LoadWAV("res/bum.wav");
        sfx_win = Mix_LoadWAV("res/ez.wav");
        font = TTF_OpenFont("res/DOTMATRI.TTF", 120);
}

void
create_board(int w, int h, int mine_count, SContext *ctx) {
        int new_w = w * 80 + 80, new_h = h * 80 + 240;
        SDL_SetWindowSize(ctx->window, new_w, new_h);
        SDL_RenderSetLogicalSize(ctx->renderer, new_w / 4, new_h / 4);
        current_board = malloc(sizeof(Board));
        current_board->game_ended = false;
        current_board->known_fields = 0;
        current_board->mines_left = mine_count;
        current_board->mine_count = mine_count;
        current_board->width = w;
        current_board->height = h;
        int i = 0;
        for (; i < w; i++) {
                int j = 0;
                for (; j < h; j++) {
                        fields[i][j] = create_field(i, j);
                }
        }
        i = 0;
        for (; i < mine_count; i++) {
                FieldType* ft = 
                        &fields[rand() % current_board->width]
                               [rand() % current_board->height]->type;
                if (*ft == Mine) { i--; continue; }
                *ft = Mine;
        }
        i = 0;
        for (; i < w; i++) {
                int j = 0;
                for (; j < h; j++) {
                        count_mines_around(fields[i][j]);
                }
        }
}

void
restart_board(int w, int h, int mine_count, SContext* ctx) {
        cleanup_fields();
        create_board(w, h, mine_count, ctx);
        current_board->game_ended = false;
}

void
end_board(bool boom) {
        if (current_board->game_ended) return;
        current_board->game_ended = true;
        if (boom) Mix_PlayChannel(-1, sfx_bum, 0);
        int i = 0;
        for (; i < current_board->width; i++) {
                int j = 0;
                for (; j < current_board->height; j++) {
                        fields[i][j]->is_hidden = false;
                }
        }
}


void
draw_board(SDL_Renderer* renderer) {
        SDL_Color text_color = { 255, 255, 255 };
        char buf[13] = { 0 };
        sprintf(buf, "%d", current_board->mines_left);
        SDL_Surface* surf_mess = 
                TTF_RenderText_Solid(font, buf, text_color);
        SDL_Texture* counter =
                SDL_CreateTextureFromSurface(renderer, surf_mess);
        SDL_Rect dest;
        TTF_SizeText(font, buf, &dest.w, &dest.h);
        dest.x = 10; dest.y = 10;
        dest.w /= 4; dest.h /= 4;
        SDL_RenderCopy(renderer, counter, NULL, &dest);
        int i = 0;
        SDL_FreeSurface(surf_mess); SDL_DestroyTexture(counter);
        for (; i < current_board->width; i++) {
                int j = 0;
                for (; j < current_board->height; j++) {
                        draw_field(renderer, fields[i][j]);
                }
        }
}

void
cleanup_fields(void) {
        int i = 0;
        for (; i < current_board->width; i++) {
                int j = 0;
                for (; j < current_board->height; j++) {
                        destroy_field(fields[i][j]);
                }
        }
        free(current_board);
}

void
mouse_field_inter(bool rmb) {
        int pos[2] = { 0 };
        mouse_pos(pos);
        pos[0] /= 4;
        pos[1] /= 4;
        int i = 0;
        for (; i < current_board->width; i++) {
                int j = 0;
                for (; j < current_board->height; j++) {
                        int fw = fields[i][j]->rect->w;
                        int fh = fields[i][j]->rect->h;
                        int fx = fw * i + 10;
                        int fy = fh * j + 50;
                        if (pos[0] >= fx && pos[0] <= fx + fw &&
                            pos[1] >= fy && pos[1] <= fy + fh) {
                                Field *f = fields[i][j];
                                if (rmb && f->is_hidden) {
                                        f->is_flagged = !f->is_flagged;
                                        f->is_flagged ? 
                                        current_board->mines_left-- :
                                        current_board->mines_left++;
                                }
                                else if (!rmb && !f->is_flagged) {
                                        if (f->is_hidden && f->type != Mine)
                                                current_board->known_fields++;
                                        f->is_hidden = false;
                                        if (f->type == Mine) end_board(true);
                                }
                        }
                }
        }
        if (current_board->known_fields >= 
                current_board->width * current_board->height -
                current_board->mine_count) {
                Mix_PlayChannel(-1, sfx_win, 0);
                end_board(false);
                current_board->mines_left = 69;
        }
}

void
count_mines_around(Field *field) {
        if (field->type == Mine) return;
        int count = 0;
        int x = (field->rect->x - 10) / field->rect->w;
        int y = (field->rect->y - 50) / field->rect->h;
        int dx = -1;
        for (; dx <= 1; dx++) {
                int dy = -1;
                for (; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx > current_board->width - 1 
                         || ny < 0 || ny > current_board->height - 1) continue;
                        if (fields[nx][ny]->type != Mine) continue;
                        count++;
                }
        }
        field->type = (FieldType)count;
}
