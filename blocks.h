//
// Created by wortelus on 23.11.21.
//


#ifndef TETRIS_BLOCKS_H
#define TETRIS_BLOCKS_H

#include <SDL2/SDL.h>

typedef struct PlayBlock {
    SDL_Rect Rect;
    int X, Y;
    int Filled;
    unsigned char R, G, B;
} PlayBlock;

typedef struct BlockBounds {
    int UL_X, UL_Y, LR_X, LR_Y;
    int Size;
}BlockBounds;

void RecreateActiveBlocks(PlayBlock* bb, PlayBlock* bb_n,
                          int* bpat, int* bpat_n,
                          int* bcol, int* bcol_n,
                          int* bx, int* by,
                          BlockBounds* abounds, BlockBounds* nbounds);

void PlayBlock_Init(PlayBlock* playBlock, int x, int y, int color);
PlayBlock* PlayBlock_PlayListInit();

void PlayBlock_ListInit(PlayBlock* playBlocks, int count, int color);
void PlayBlockSetColor(PlayBlock* playBlock, int color);
void PlayBlockSetPosition(PlayBlock* playBlock, int x, int y);
void PlayBlockSetRectPosition(PlayBlock* playBlock);
void PlayBlockActiveUpdatePosition(PlayBlock* activePlayBlocks, int bflag, int by);
void PlayBlockActiveGenerate(PlayBlock* playBlocks, int pattern, int color, BlockBounds* bounds);
void PlayBlockActiveRotate(PlayBlock* activePlayBlocks, int bx, int by, BlockBounds* bounds, int swapBounds);
void ActivePlayBlockCopy(PlayBlock* dst, PlayBlock* src);
void PlayBlockCopyParameters(PlayBlock* dst, PlayBlock* src);
void PlayBlockClearout(PlayBlock* playBlock);
void PlayBlockCopy(PlayBlock* dst, PlayBlock* src);

#endif //TETRIS_BLOCKS_H
