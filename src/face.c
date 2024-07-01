#include <face.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <common.h>
#include <stdlib.h>
#include <time.h>

Face*
init_face(SContext* ctx) {
        SDL_Renderer* renderer = ctx->renderer;
        Face* face = malloc(sizeof(Face));
        face->face_dest = malloc(sizeof(SDL_Rect));
        int win_w;
        SDL_GetWindowSize(ctx->window, &win_w, NULL);
        face->face_dest->w = 30; face->face_dest->h = 30;
        face->face_dest->x = win_w / 4 - 10 - face->face_dest->w;
        face->face_dest->y = face->face_dest->h / 2 - 3;
        face->current = Idle;
        face->textures = malloc(sizeof(FaceTextures));
        face->textures->idle =
                IMG_LoadTexture(renderer, "res/face_idle.png");
        face->textures->reveal = 
                IMG_LoadTexture(renderer, "res/face_reveal.png");
        face->textures->dead = 
                IMG_LoadTexture(renderer, "res/face_dead.png");
        face->textures->win = 
                IMG_LoadTexture(renderer, "res/face_win.png");
        return face;
}

SDL_Texture*
set_face_texture(FaceType face_type, FaceTextures* textures) {
        switch (face_type) {
                case Idle: return textures->idle;
                case Reveal: return textures->reveal;
                case Dead: return textures->dead;
                case Win: return textures->win;
                default: return textures->idle;
        }
}

clock_t tt_change = 0;
void
change_face(FaceType face_type, FaceType next, 
            FaceType* current, bool check_for_r) {
        if (!check_for_r) {
                *current = face_type;
                if (face_type == Reveal) 
                        tt_change = clock() + 75000;
        }
        else if (tt_change > 0) {
                if (tt_change <= clock()) {
                        *current = next > -1 ? next : Idle;
                        tt_change = 0;
                }
        }
}

void
draw_face(SContext* ctx, Face* face) {
        SDL_RenderCopy(
                ctx->renderer, 
                set_face_texture(face->current, face->textures), 
                NULL, 
                face->face_dest
        );
}

void
cleanup_face(Face* face) {
        SDL_DestroyTexture(face->textures->idle);
        SDL_DestroyTexture(face->textures->reveal);
        SDL_DestroyTexture(face->textures->dead);
        SDL_DestroyTexture(face->textures->win);
        free(face->textures);
        free(face->face_dest);
        free(face);
}
