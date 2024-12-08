//
// Created by wortelus on 16.11.21.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "blocks.h"
#include "render.h"

void Render_SetBackground(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Render_DrawBlocks(SDL_Renderer* renderer, PlayBlock* blocks, int count) {
    for (int i = 0; i < count; i++) {
        SDL_SetRenderDrawColor(renderer, blocks[i].R, blocks[i].G, blocks[i].B, 255);
        SDL_RenderFillRect(renderer, &blocks[i].Rect);
    }
}

void Render_DrawScore(SDL_Renderer* renderer, int score, TTF_Font* font) {
    char label[10];
    sprintf(label, "SCORE: %d", score);
    SDL_Surface* surface = TTF_RenderText_Solid(font,
                                                 label, textcolor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int tw = 0; int th = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect textrect = {SCORE_LABEL_X, SCORE_LABEL_Y, tw, th};
    SDL_RenderCopy(renderer, texture, NULL, &textrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Render_DrawText(SDL_Renderer* renderer, TTF_Font* font) {
    char label[12];
    sprintf(label, "NEXT BLOCK:");
    SDL_Surface* surface = TTF_RenderText_Solid(font,
                                                label, textcolor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int tw = 0; int th = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect textrect = {PLAY_NEXT_ACTIVE_BLOCK_LABEL_X, PLAY_NEXT_ACTIVE_BLOCK_LABEL_Y, tw, th};
    SDL_RenderCopy(renderer, texture, NULL, &textrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Render_DrawYouLostText(SDL_Renderer* renderer, TTF_Font* font) {
    char label[12];
    sprintf(label, "YOU LOST :(");
    SDL_Surface* surface = TTF_RenderText_Solid(font,
                                                label, textcolor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int tw = 0; int th = 0;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect textrect = {YOU_LOST_LABEL_X, YOU_LOST_LABEL_Y, tw, th};
    SDL_RenderCopy(renderer, texture, NULL, &textrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Render_DrawActiveBlocks(SDL_Renderer* renderer, PlayBlock* blocks) {
    for (int i = 0; i < 4; i++) {
        SDL_SetRenderDrawColor(renderer, blocks[i].R, blocks[i].G, blocks[i].B, 255);
        SDL_RenderFillRect(renderer, &blocks[i].Rect);
    }
}

void Render_DrawNextActiveBlocks(SDL_Renderer* renderer, PlayBlock* blocks, int count) {
    for (int i = 0; i < count; i++) {
        SDL_SetRenderDrawColor(renderer, blocks[i].R, blocks[i].G, blocks[i].B, 255);
        PlayBlock bl = blocks[i];
        blocks[i].Rect.x = PLAY_NEXT_ACTIVE_BLOCK_POSITION_X + bl.X * (BLOCK_SIZE + BLOCK_GAP_SIZE);
        blocks[i].Rect.y = PLAY_NEXT_ACTIVE_BLOCK_POSITION_Y + bl.Y * (BLOCK_SIZE + BLOCK_GAP_SIZE);
        SDL_RenderFillRect(renderer, &blocks[i].Rect);
    }
}
