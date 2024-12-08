//
// Created by wortelus on 23.11.21.
//

#include <SDL2/SDL.h>

#include "blocks.h"
#include "colors.h"
#include "constants.h"

// init single play block instance
void PlayBlock_Init(PlayBlock* playBlock, int x, int y, int color) {
    playBlock->X = x;
    playBlock->Y = y;

    playBlock->Filled = 0;

    playBlock->Rect.x = x*(BLOCK_SIZE+BLOCK_GAP_SIZE);
    playBlock->Rect.y = y*(BLOCK_SIZE+BLOCK_GAP_SIZE);
    playBlock->Rect.w = BLOCK_SIZE; playBlock->Rect.h = BLOCK_SIZE;

    unsigned char col[3];
    GetColor(col, color);
    playBlock->R = col[0];
    playBlock->G = col[1];
    playBlock->B = col[2];
}

// initiate and set default values of play block field
PlayBlock* PlayBlock_PlayListInit() {
    PlayBlock* blocks;
    blocks = (PlayBlock*)malloc(sizeof(PlayBlock) * PLAY_BLOCK_COUNT);
    for (int y = 0; y < PLAY_BLOCK_HEIGHT; y++) {
        for (int x = 0; x < PLAY_BLOCK_WIDTH; x++) {
            PlayBlock* block = &blocks[y * PLAY_BLOCK_WIDTH + x];
            PlayBlock_Init(block, x, y, 0);
        }
    }
    return blocks;
}

// update the loop after the active block has been fixed into position
void RecreateActiveBlocks(PlayBlock* bb, PlayBlock* bb_n,
                          int* bpat, int* bpat_n,
                          int* bcol, int* bcol_n,
                          int* bx, int* by,
                          BlockBounds* abounds, BlockBounds* nbounds) {

    // copy new block values and discard the old
    *bx = rand() % (PLAY_BLOCK_WIDTH - nbounds->LR_X + 1);
    *by = 0; // offset because in the main loop the by++ is called afterwards
    *bpat = *bpat_n;
    *bpat_n = rand() % PATTERN_COUNT;
    *bcol = *bcol_n;
    *bcol_n = 1 + rand() % (PLAY_COLORS_COUNT-1); // offset because the 0 color is the background color {128, 128, 128}
    *abounds = *nbounds;

    // copy bb_n into bb
    for(int i = 0; i < 4; i++) {
        PlayBlock* pb = &bb[i];
        PlayBlockCopy(pb, &bb_n[i]);
        PlayBlockSetPosition(pb, pb->X + *bx, pb->Y);
        PlayBlockSetRectPosition(pb);
    }

    // and finally, the next block generation
    PlayBlockActiveGenerate(bb_n, *bpat_n, *bcol_n, nbounds);
}

// initiate array of play blocks given the count and their default color
void PlayBlock_ListInit(PlayBlock* playBlocks, int count, int color) {
    for (int i = 0; i < count; i++) {
        PlayBlock *block = &playBlocks[i];
        PlayBlock_Init(block, 0, 0, color);
    }
}


void PlayBlockSetColor(PlayBlock* playBlock, int color) {
    unsigned char col[3];
    GetColor(col, color);
    playBlock->R = col[0];
    playBlock->G = col[1];
    playBlock->B = col[2];
}

void ActivePlayBlockCopy(PlayBlock* dst, PlayBlock* src) {
    for(int i = 0; i < 4; i++) {
        PlayBlockCopy(&dst[i], &src[i]);
    }
}

void PlayBlockCopy(PlayBlock* dst, PlayBlock* src) {
    dst->R = src->R;
    dst->G = src->G;
    dst->B = src->B;
    dst->X = src->X;
    dst->Y = src->Y;
    dst->Rect.x = src->Rect.x;
    dst->Rect.y = src->Rect.y;
    dst->Rect.w = src->Rect.w;
    dst->Rect.h = src->Rect.h;
    dst->Filled = src->Filled;
}

void PlayBlockCopyParameters(PlayBlock* dst, PlayBlock* src) {
    dst->R = src->R;
    dst->G = src->G;
    dst->B = src->B;
    dst->Rect.w = src->Rect.w;
    dst->Rect.h = src->Rect.h;
    dst->Filled = src->Filled;
}

void PlayBlockClearout(PlayBlock* playBlock) {
    playBlock->R = BLOCK_COLOR_R;
    playBlock->G = BLOCK_COLOR_G;
    playBlock->B = BLOCK_COLOR_B;
    playBlock->Filled = 0;
}

void PlayBlockSetPosition(PlayBlock* playBlock, int x, int y) {
    playBlock->X = x;
    playBlock->Y = y;
}

void PlayBlockSetRectPosition(PlayBlock* playBlock) {
    int x = playBlock->X;
    int y = playBlock->Y;

    playBlock->Rect.x = x * (BLOCK_SIZE + BLOCK_GAP_SIZE);
    playBlock->Rect.y = y * (BLOCK_SIZE + BLOCK_GAP_SIZE);
}

void PlayBlockActiveUpdatePosition(PlayBlock* activePlayBlocks, int bflag, int by) {
    for (int i = 0; i < 4; i++) {
        PlayBlock *pb = &activePlayBlocks[i];
        PlayBlockSetPosition(pb, pb->X + bflag, pb->Y + by);
        PlayBlockSetRectPosition(pb);
    }
}

void rotate_bounds(BlockBounds* bounds) {
    BlockBounds b = *bounds;
    bounds->UL_X = bounds->Size - b.LR_Y;
    bounds->UL_Y = b.UL_X;
    bounds->LR_X = bounds->Size - b.UL_Y;
    bounds->LR_Y = b.LR_X;
}

// rotate active block
void PlayBlockActiveRotate(PlayBlock* activePlayBlocks, int bx, int by, BlockBounds* bounds, int swapBounds) {
    for (int i = 0; i < 4; i++) {
        PlayBlock *pb = &activePlayBlocks[i];
        // inner-block positions
        int dx = pb->X - bx;
        int dy = pb->Y - by;
        // transpose
        PlayBlockSetPosition(pb, bx + dy, by + dx);
        // update positions
        dx = pb->X - bx;
        dy = pb->Y - by;
        // rotate
        PlayBlockSetPosition(pb, bx + (bounds->Size - dx - 1), by + dy);
        PlayBlockSetRectPosition(pb);
    }

    // used for rotate testing
    if (swapBounds) {
        rotate_bounds(bounds);
    }
}

void PlayBlockActiveGenerate(PlayBlock* playBlocks, int pattern, int color, BlockBounds* bounds) {
    // precaution
    color %= PLAY_COLORS_COUNT;
    pattern %= PATTERN_COUNT;

    PlayBlockSetColor(&playBlocks[0], color);
    PlayBlockSetColor(&playBlocks[1], color);
    PlayBlockSetColor(&playBlocks[2], color);
    PlayBlockSetColor(&playBlocks[3], color);

    switch (pattern) {
        case 0:
            PlayBlockSetPosition(&playBlocks[0], 0, 1);
            PlayBlockSetPosition(&playBlocks[1], 1, 1);
            PlayBlockSetPosition(&playBlocks[2], 2, 1);
            PlayBlockSetPosition(&playBlocks[3], 3, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 1;
            bounds->LR_X = 4;
            bounds->LR_Y = 2;
            bounds->Size = 4;
            break;
        case 1:
            PlayBlockSetPosition(&playBlocks[0], 0, 0);
            PlayBlockSetPosition(&playBlocks[1], 0, 1);
            PlayBlockSetPosition(&playBlocks[2], 1, 1);
            PlayBlockSetPosition(&playBlocks[3], 2, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 3;
            bounds->LR_Y = 2;
            bounds->Size = 3;
            break;
        case 2:
            PlayBlockSetPosition(&playBlocks[0], 2, 0);
            PlayBlockSetPosition(&playBlocks[1], 0, 1);
            PlayBlockSetPosition(&playBlocks[2], 1, 1);
            PlayBlockSetPosition(&playBlocks[3], 2, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 3;
            bounds->LR_Y = 2;
            bounds->Size = 3;
            break;
        case 3:
            PlayBlockSetPosition(&playBlocks[0], 0, 0);
            PlayBlockSetPosition(&playBlocks[1], 0, 1);
            PlayBlockSetPosition(&playBlocks[2], 1, 0);
            PlayBlockSetPosition(&playBlocks[3], 1, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 2;
            bounds->LR_Y = 2;
            bounds->Size = 2;
            break;
        case 4:
            PlayBlockSetPosition(&playBlocks[0], 0, 1);
            PlayBlockSetPosition(&playBlocks[1], 1, 1);
            PlayBlockSetPosition(&playBlocks[2], 1, 0);
            PlayBlockSetPosition(&playBlocks[3], 2, 0);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 3;
            bounds->LR_Y = 2;
            bounds->Size = 3;
            break;
        case 5:
            PlayBlockSetPosition(&playBlocks[0], 1, 0);
            PlayBlockSetPosition(&playBlocks[1], 0, 1);
            PlayBlockSetPosition(&playBlocks[2], 1, 1);
            PlayBlockSetPosition(&playBlocks[3], 2, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 3;
            bounds->LR_Y = 2;
            bounds->Size = 3;
            break;
        default:
            PlayBlockSetPosition(&playBlocks[0], 0, 0);
            PlayBlockSetPosition(&playBlocks[1], 1, 0);
            PlayBlockSetPosition(&playBlocks[2], 1, 1);
            PlayBlockSetPosition(&playBlocks[3], 2, 1);
            bounds->UL_X = 0;
            bounds->UL_Y = 0;
            bounds->LR_X = 3;
            bounds->LR_Y = 2;
            bounds->Size = 3;
            break;
    }
}