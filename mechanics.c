//
// Created by wortelus on 08.12.21.
//

#include "mechanics.h"
#include "blocks.h"
#include "constants.h"

int CanBePlaced(PlayBlock* gameblocks, PlayBlock* activeblocks) {
    for (int i = 0; i < 4; i++) {
        PlayBlock pb = activeblocks[i];
        int pos = (pb.Y + 0) * PLAY_BLOCK_WIDTH + pb.X;

        if (IsFilled(gameblocks[pos])) {
            return 0;
        }
    }
    return 1;
}

int HasLost(PlayBlock* gameblocks, PlayBlock* activeblocks, int* bx, BlockBounds bounds) {
    for(int x = 0; x < PLAY_BLOCK_WIDTH - bounds.LR_X + 1; x++) {
        // initial value to 1
        int found = 1;
        for (int i = 0; i < 4; i++) {
            PlayBlock pb = activeblocks[i];
            int pos = (pb.Y + 0) * PLAY_BLOCK_WIDTH + (pb.X - *bx + x);

            if(!IsValidGameBlock(pb.X - *bx + x, (pb.Y + 0)) || IsFilled(gameblocks[pos])) {
                found = 0;
                break;
            }
        }
        if (found) {
            return x;
        }
    }

    // player has lost
    return -1;
}

// 1 - block has been fixed and copied into the game block field
// 0 - the active block set remains free to move
int TryToFixActiveBlock(PlayBlock* gameblocks, PlayBlock* activeblocks) {
    for (int i = 0; i < 4; i++) {
        if(GetBlockUnderInfo(gameblocks, activeblocks[i])) {
            ImprintActiveBlocks(gameblocks, activeblocks);
            return 1;
        }
    }
    return 0;
}

// check for obstacles before rotating
int CanRotateActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks, int bx, int by, BlockBounds* bounds) {
    PlayBlock copiedBlocks[4];
    ActivePlayBlockCopy(copiedBlocks, activeblocks);
    PlayBlockActiveRotate(copiedBlocks, bx, by, bounds, 0);

    for (int i = 0; i < 4; i++) {
        PlayBlock pb = copiedBlocks[i];
        int pos = pb.Y * PLAY_BLOCK_WIDTH + pb.X;
        if(!IsValidGameBlock(pb.X, pb.Y) || IsFilled(gameblocks[pos])) {
            return 0;
        }
    }
    return 1;
}

// check for obstacles before moving left/right up/down
int CanMoveActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks, int xflag, int yflag) {
    for (int i = 0; i < 4; i++) {
        PlayBlock pb = activeblocks[i];
        int pos = (pb.Y + yflag) * PLAY_BLOCK_WIDTH + (pb.X + xflag);
        if(!IsValidGameBlock(pb.X, pb.Y) || IsFilled(gameblocks[pos])) {
            return 0;
        }
    }
    return 1;
}

// delete complete lines, returns the number of deleted lines
int DeleteLines(PlayBlock* gameblocks) {
    int y = PLAY_BLOCK_HEIGHT - 1;
    int count = 0;

    while ((y = IsCreatedLine(gameblocks, y)) > 0) {
        DeleteLine(gameblocks, y);
        count++;
    }

    return count;
}

void DeleteLine(PlayBlock* gameblocks, int yline) {
    for (int y = yline; y > 1; y--) {
        for (int x = 0; x < PLAY_BLOCK_WIDTH; x++) {
            MoveBlockDown(gameblocks, x, y);
        }
    }
}

void MoveBlockDown(PlayBlock* gameblocks, int x, int y) {
    PlayBlock* upper = &gameblocks[(y-1) * PLAY_BLOCK_WIDTH + x];
    PlayBlock* pb = &gameblocks[y * PLAY_BLOCK_WIDTH + x];
    PlayBlockCopyParameters(pb, upper);
    PlayBlockClearout(upper);
}

// y is used for starting where left off after line deletion
int IsCreatedLine(PlayBlock* gameblocks, int y) {
    for(; y > 1; y--) {
        int x = 0;
        for (; x < PLAY_BLOCK_WIDTH; x++) {
            if (!IsFilled(gameblocks[y * PLAY_BLOCK_WIDTH + x])) {
                break;
            }
        }
        if (x == PLAY_BLOCK_WIDTH) {
            return y;
        }
    }

    return -1;
}

void ImprintActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks) {
    for (int i = 0; i < 4; i++) {
        PlayBlock* pb = &gameblocks[activeblocks[i].Y * PLAY_BLOCK_WIDTH + activeblocks[i].X];
        PlayBlockCopy(pb, &activeblocks[i]);
        pb->Filled = 1;
    }
}

// 2 for nonexistent, 1 for filled, 0 for free
int GetBlockUnderInfo(PlayBlock* gameblocks, PlayBlock activeblock) {
    int x = activeblock.X;
    int y = activeblock.Y;
    int ypp = y + 1;
    if (!IsValidGameBlock(x, ypp)) {
        return 2;
    }
    if (IsFilled(gameblocks[ypp * PLAY_BLOCK_WIDTH + x])) {
        return 1;
    }
    return 0;
}

int IsValidGameBlock(int x, int y) {
    if (x >= PLAY_BLOCK_WIDTH || y >= PLAY_BLOCK_HEIGHT || x < 0 || y < 0) {
            return 0;
    }
    return 1;
}

int IsFilled(PlayBlock gameblock) {
    return gameblock.Filled;
}