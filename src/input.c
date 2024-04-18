#include <input.h>
#include <SDL2/SDL.h>
#include <common.h>
#include <stdbool.h>
#include <memory.h>

bool keys[SDL_NUM_SCANCODES] = { false },
     prev_keys[SDL_NUM_SCANCODES] = { false },
     lmb[2] = { false }, rmb[2] = { false };

void
copy_input_array(void) {
        memcpy(prev_keys, keys, sizeof(keys));
        lmb[1] = lmb[0];
        rmb[1] = rmb[0];
}

void 
update_input(SContext *ctx) {
        if (ctx->event.key.keysym.sym > SDL_NUM_SCANCODES
                || ctx->event.key.repeat != 0) return;
        switch(ctx->event.type) {
                case SDL_KEYDOWN: {
                        keys[ctx->event.key.keysym.sym] = true; 
                        break;
                }
                case SDL_KEYUP: {
                        keys[ctx->event.key.keysym.sym] = false; 
                        break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                        if (ctx->event.button.button == SDL_BUTTON_LEFT)
                                lmb[0] = true;
                        else if (ctx->event.button.button == SDL_BUTTON_RIGHT)
                                rmb[0] = true;
                        break;
                }
                case SDL_MOUSEBUTTONUP: {
                        if (ctx->event.button.button == SDL_BUTTON_LEFT)
                                lmb[0] = false;
                        else if (ctx->event.button.button == SDL_BUTTON_RIGHT)
                                rmb[0] = false;
                        break;
                }
        }
}

const bool
key_pressed(int key) {
        return keys[key] && !prev_keys[key];
}

const bool 
key_held(int key) {
        return keys[key];
}

const bool 
lmb_pressed(void) {
        return lmb[0] && !lmb[1];
}

const bool 
lmb_held(void) {
        return lmb[0];
}

const bool 
rmb_pressed(void) {
        return rmb[0] && !rmb[1];
}

const bool 
rmb_held(void) {
        return rmb[0];
}

void
mouse_pos(int *buf) {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);
        buf[0] = x; buf[1] = y;
}
