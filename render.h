//
// Created by wortelus on 16.11.21.
//

#ifndef TETRIS_RENDER_H
#define TETRIS_RENDER_H

#include "blocks.h"

static const SDL_Color textcolor = { .r =  255, .g = 255, .b = 255 };

void Render_SetBackground(SDL_Renderer* renderer);
void Render_DrawScore(SDL_Renderer* renderer, int score, TTF_Font* font);
void Render_DrawText(SDL_Renderer* renderer, TTF_Font* font);
void Render_DrawYouLostText(SDL_Renderer* renderer, TTF_Font* font);
void Render_DrawBlocks(SDL_Renderer* renderer, PlayBlock* blocks, int count);
void Render_DrawActiveBlocks(SDL_Renderer* renderer, PlayBlock* blocks);
void Render_DrawNextActiveBlocks(SDL_Renderer* renderer, PlayBlock* blocks, int count);
#endif //TETRIS_RENDER_H
