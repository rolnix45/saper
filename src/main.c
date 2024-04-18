#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <field.h>
#include <board.h>
#include <common.h>
#include <input.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

SContext*
init(void) {
        assert(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
        assert(!TTF_Init());
        SContext* ctx = malloc(sizeof(SContext));
        ctx->window = SDL_CreateWindow(
                "saper", 
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                1024, 1024, 0
        );
        assert(ctx->window);
        assert(!Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024));
        ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);
        SDL_RenderSetScale(ctx->renderer, 4, 4);
        SDL_RenderSetLogicalSize(ctx->renderer, 1024 / 4, 1024 / 4);
        ctx->close_window = false;
        ctx->arg_board_w = 5;
        ctx->arg_board_h = 5;
        ctx->arg_mine_count = 5;
        ctx->arbuz = false;
        load_textures(ctx->renderer);
        load_board_res();
        return ctx;
}

void
loop(SContext* ctx) {
        copy_input_array();
        while (SDL_PollEvent(&ctx->event) > 0) {
                update_input(ctx);
                if (ctx->event.type == SDL_QUIT) ctx->close_window = true;
        }
        if (key_pressed(SDLK_ESCAPE)) ctx->close_window = true;
        if (key_pressed(SDLK_r)) restart_board(ctx->arg_board_w, ctx->arg_board_h, ctx->arg_mine_count, ctx);
        if (key_pressed(SDLK_e)) end_board(false);
        if (key_pressed(SDLK_w)) end_board(true);
        if (lmb_pressed()) mouse_field_inter(false);
        if (rmb_pressed()) mouse_field_inter(true);
        SDL_SetRenderDrawColor(ctx->renderer, 20, 0, 60, 255);
        SDL_RenderClear(ctx->renderer);
        draw_board(ctx->renderer);
        SDL_RenderPresent(ctx->renderer);
}

void
handle_args(int argc, char** argv, SContext* ctx) {
        int e = 0;
        for (; e < argc; e++) {
                if (!strcmp(argv[e], "-w")) {
                        int w = atoi(argv[e + 1]);
                        if (w == 0) return;
                        ctx->arg_board_w = w;
                }

                else if (!strcmp(argv[e], "-h")) {
                        int h = atoi(argv[e + 1]);
                        if (h == 0) return;
                        ctx->arg_board_h = h;
                }

                else if (!strcmp(argv[e], "-m")) {
                        int m = atoi(argv[e + 1]);
                        if (m == 0) return;
                        if (m > ctx->arg_board_w * ctx->arg_board_h) return;
                        ctx->arg_mine_count = m;
                }

                else if (!strcmp(argv[e], "-arbuz")) {
                        ctx->arbuz = true;
                        switch_texture();
                }
        }
}

int
main(int argc, char** argv) {
        SContext* ctx = init();
        assert(ctx); 
        handle_args(argc, argv, ctx);
        create_board(ctx->arg_board_w, ctx->arg_board_h, ctx->arg_mine_count, ctx);
        while (!ctx->close_window) loop(ctx);
        cleanup_fields();
        free(ctx);
        TTF_Quit();
        IMG_Quit();
        Mix_Quit();
        SDL_Quit();
        return 0;
}
