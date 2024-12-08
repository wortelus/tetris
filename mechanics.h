//
// Created by wortelus on 08.12.21.
//

#ifndef TETRIS_MECHANICS_H
#define TETRIS_MECHANICS_H

#include "blocks.h"

int CanBePlaced(PlayBlock* gameblocks, PlayBlock* activeblocks);
int HasLost(PlayBlock* gameblocks, PlayBlock* activeblocks, int* bx, BlockBounds bounds);

int TryToFixActiveBlock(PlayBlock* gameblocks, PlayBlock* activeblocks);
int CanRotateActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks, int bx, int by, BlockBounds* bounds);
int CanMoveActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks, int xflag, int yflag);

int DeleteLines(PlayBlock* gameblocks);
void DeleteLine(PlayBlock* gameblocks, int yline);
void MoveBlockDown(PlayBlock* gameblocks, int x, int y);

int IsCreatedLine(PlayBlock* gameblocks, int y);
int GetBlockUnderInfo(PlayBlock* gameblocks, PlayBlock activeblock);
void ImprintActiveBlocks(PlayBlock* gameblocks, PlayBlock* activeblocks);
int IsValidGameBlock(int x, int y);
int IsFilled(PlayBlock gameblock);

#endif //TETRIS_MECHANICS_H
