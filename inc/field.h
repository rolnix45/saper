#ifndef S_FIELD_H
#define S_FIELD_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <common.h>

typedef enum {
        Empty,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Mine
} FieldType;

typedef struct {
        SDL_Rect* rect;
        SDL_Texture* texture;
        FieldType type;
        bool is_hidden;
        bool is_flagged;
} Field;

void load_textures(SDL_Renderer* renderer);
void switch_texture(void);
Field* create_field(int x, int y, 
        int board_w, int board_h, 
        int new_w, int new_h);
void draw_field(SDL_Renderer* renderer, Field* field);
void destroy_field(Field* field);

#endif
