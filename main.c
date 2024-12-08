#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
int nanosleep(const struct timespec *req, struct timespec *rem);

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "render.h"
#include "mechanics.h"
#include "blocks.h"
#include "colors.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("error loading sdl.");
        return 1;
    }
    if (TTF_Init() < 0) {
        printf("error loading ttf_init sdl module.");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
            "Wortelus Tetris",
            100,
            100,
            TOTAL_WIDTH,
            TOTAL_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
    );

//     TTF_Font *font = TTF_OpenFont("/home/wortelus/Stažené/FreeSerif.ttf", 25);
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSerif.ttf", 25);
    if (font == NULL) {
        printf("loading font error: %s", TTF_GetError());
        return 1;
    }

    // initialize random generators with time seed
    srand(time(NULL));

    // play field - dynamically allocated
    PlayBlock *blocks = PlayBlock_PlayListInit();

    int score = 0; int lost = 0;
    int bx = 0, by = 0, bflag = 0;

    // get pattern/color of active and next block
    int bpat = rand() % PATTERN_COUNT, bpat_n = rand() % PATTERN_COUNT;
    int bcol = bcol = 1 + rand() % (PLAY_COLORS_COUNT - 1), bcol_n = 1 + rand() % (PLAY_COLORS_COUNT - 1);

    // --- initialize active/next block
    PlayBlock bb[4];
    BlockBounds abounds;
    PlayBlock_ListInit(bb, 4, bcol);
    PlayBlockActiveGenerate(bb, bpat, bcol, &abounds);
    bx = rand() % (PLAY_BLOCK_WIDTH - abounds.LR_X + 1);
    // update active block position
    for (int i = 0; i < 4; i++) {
        PlayBlock *pb = &bb[i];
        PlayBlockSetPosition(pb, pb->X + bx, pb->Y);
        PlayBlockSetRectPosition(pb);
    }
    // next blocks
    PlayBlock bb_n[4];
    BlockBounds nbounds;
    PlayBlock_ListInit(bb_n, 4, bcol_n);
    PlayBlockActiveGenerate(bb_n, bpat_n, bcol_n, &nbounds);
    // ---


    // --- timespec for sleeping the left-right moving period
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = MILLISECONDS_LR_PERIOD_SLEEP * 1000000;
    // ---

    bool quit = false;
    while (!quit) {
        time_t start, end;
        double elapsed;  // seconds
        start = time(NULL);
        int terminate = 1;
        while (terminate) {

            //SECONDS_SLEEP checker for loop while(terminate)
            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= SECONDS_SLEEP /* seconds */)
                terminate = 0;

            // key-down handler, while loop for processing all pending events
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        quit = true;
                        terminate = 0;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_LEFT:
                                if (bx + abounds.UL_X > 0) {
                                    if (!CanMoveActiveBlocks(blocks, bb, -1, 0)) {
                                        continue;
                                    }
                                    bx--;
                                    bflag = -1;
                                    PlayBlockActiveUpdatePosition(bb, bflag, 0);
                                    bflag = 0;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (bx < PLAY_BLOCK_WIDTH - abounds.LR_X) {
                                    if (!CanMoveActiveBlocks(blocks, bb, 1, 0)) {
                                        continue;
                                    }
                                    bx++;
                                    bflag = 1;
                                    PlayBlockActiveUpdatePosition(bb, bflag, 0);
                                    bflag = 0;
                                }
                                break;
                            case SDLK_UP:
                                if (CanRotateActiveBlocks(blocks, bb, bx, by, &abounds)) {
                                    PlayBlockActiveRotate(bb, bx, by, &abounds, 1);
                                }
                                break;
                            case SDLK_DOWN:
                                terminate = 0;
                                break;
                        }
                        Render_SetBackground(renderer);
                        Render_DrawBlocks(renderer, blocks, PLAY_BLOCK_COUNT);
                        Render_DrawActiveBlocks(renderer, bb);
                        Render_DrawNextActiveBlocks(renderer, bb_n, 4);
                        Render_DrawScore(renderer, score, font);
                        Render_DrawText(renderer, font);
                        SDL_RenderPresent(renderer);
                        break;
                    default:
                        continue;
                }
            }
        }

        if (TryToFixActiveBlock(blocks, bb)) {
            RecreateActiveBlocks(bb, bb_n, &bpat, &bpat_n, &bcol, &bcol_n, &bx, &by, &abounds, &nbounds);
        }

        PlayBlockActiveUpdatePosition(bb, 0, 1);

        score += DeleteLines(blocks);

        if (!CanBePlaced(blocks, bb)) {
            int p = HasLost(blocks, bb, &bx, abounds);
            if (p < 0) {
                // player has successfully lost the game
                lost = 1;
                quit = 1;
            } else {
                // reposition to a possible location
                PlayBlockActiveUpdatePosition(bb, -bx + p, by);
                bx = p;
            }
        }

        Render_SetBackground(renderer);
        Render_DrawBlocks(renderer, blocks, PLAY_BLOCK_COUNT);
        if (!lost) {
            Render_DrawActiveBlocks(renderer, bb);
        }
        Render_DrawNextActiveBlocks(renderer, bb_n, 4);
        Render_DrawScore(renderer, score, font);
        Render_DrawText(renderer, font);
        SDL_RenderPresent(renderer);

        nanosleep(&tim, &tim2);
        by++;
    }

    if (lost) {
        Render_DrawYouLostText(renderer, font);
        SDL_RenderPresent(renderer);
        nanosleep(&(struct timespec) {3, 0},
                  &(struct timespec) {0, 0});
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    free(blocks);
    blocks = NULL;
    return 0;
}