#include <field.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h>

SDL_Texture* txt_hidden;
SDL_Texture* txt_empty;
SDL_Texture* txt_one;
SDL_Texture* txt_two;
SDL_Texture* txt_thr;
SDL_Texture* txt_fou;
SDL_Texture* txt_fiv;
SDL_Texture* txt_six;
SDL_Texture* txt_sev;
SDL_Texture* txt_eig;
SDL_Texture* txt_bomb;
SDL_Texture* txt_null;
SDL_Texture* txt_flag;
SDL_Texture* txt_arbuz;

void
load_textures(SDL_Renderer* renderer) {
        txt_null = IMG_LoadTexture(renderer, "res/field_nul.png");
        txt_hidden = IMG_LoadTexture(renderer, "res/field_hid.png");
        txt_empty = IMG_LoadTexture(renderer, "res/field.png");
        txt_one = IMG_LoadTexture(renderer, "res/field_one.png");
        txt_two = IMG_LoadTexture(renderer, "res/field_two.png");
        txt_thr = IMG_LoadTexture(renderer, "res/field_thr.png");
        txt_fou = IMG_LoadTexture(renderer, "res/field_fou.png");
        txt_fiv = IMG_LoadTexture(renderer, "res/field_fiv.png");
        txt_six = IMG_LoadTexture(renderer, "res/field_six.png");
        txt_sev = IMG_LoadTexture(renderer, "res/field_sev.png");
        txt_eig = IMG_LoadTexture(renderer, "res/field_eig.png");
        txt_flag = IMG_LoadTexture(renderer, "res/flag.png");
        txt_bomb = IMG_LoadTexture(renderer, "res/bomba.png");
        txt_arbuz = IMG_LoadTexture(renderer, "res/ARBUZ.png");
        srand(time(NULL));
}

void
switch_texture(void) {
        SDL_DestroyTexture(txt_bomb);
        txt_bomb = txt_arbuz;
}

SDL_Texture* 
set_texture(int num) {
        switch (num) {
                case 0: return txt_empty;
                case 1: return txt_one;
                case 2: return txt_two;
                case 3: return txt_thr;
                case 4: return txt_fou;
                case 5: return txt_fiv;
                case 6: return txt_six;
                case 7: return txt_sev;
                case 8: return txt_eig;
                case 9: return txt_bomb;
                default: return txt_null;
        }
}

Field* 
create_field(int x, int y, int board_w, int board_h, int scr_w, int scr_h) {
        Field* field = malloc(sizeof(Field));
        field->rect = malloc(sizeof(SDL_Rect));
        //int space = (scr_w / board_w) - (X_FIELD_OFFSET * 2);
        //printf("%d\n", space);
        field->rect->w = FIELD_SIZE;
        field->rect->h = field->rect->w;
        field->rect->x = field->rect->w * x + X_FIELD_OFFSET;
        field->rect->y = field->rect->h * y + Y_FIELD_OFFSET;
        field->type = Empty;
        field->is_hidden = true;
        field->is_flagged = false;
        field->texture = txt_empty;
        return field;
}

void 
draw_field(SDL_Renderer* renderer, Field* field) {
        if (field->is_hidden && field->is_flagged) 
                field->texture = txt_flag;
        else if (field->is_hidden) 
                field->texture = txt_hidden;
        else 
                field->texture = set_texture((int)field->type);
        SDL_RenderCopy(renderer, field->texture, NULL, field->rect);
}

void
destroy_field(Field* field) {
        free(field);
}
