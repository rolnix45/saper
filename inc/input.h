#ifndef S_INPUT_H
#define S_INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <common.h>

void copy_input_array(void);
void update_input(SContext *ctx);
const bool key_pressed(int key);
const bool key_held(int key);
const bool lmb_pressed(void);
const bool lmb_held(void);
const bool rmb_pressed(void);
const bool rmb_held(void);
void mouse_pos(int *buf);

#endif
