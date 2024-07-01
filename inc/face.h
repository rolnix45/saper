#ifndef S_FACE_H
#define S_FACE_H

#include <common.h>
#include <stdbool.h>

typedef enum {
        Idle,
        Reveal,
        Dead,
        Win
} FaceType;

typedef struct {
        SDL_Texture* idle;
        SDL_Texture* reveal;
        SDL_Texture* dead;
        SDL_Texture* win;
} FaceTextures;

typedef struct {
        FaceType current;
        FaceTextures* textures;
        SDL_Rect* face_dest;
} Face;

Face* init_face(SContext* ctx);
void change_face(FaceType face_type, FaceType next,
                 FaceType* current, bool check_for_r);
void draw_face(SContext* ctx, Face* face);
void cleanup_face(Face* face);

#endif
