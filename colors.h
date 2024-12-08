//
// Created by wortelus on 23.11.21.
//

#ifndef TETRIS_COLORS_H
#define TETRIS_COLORS_H

static const int BLOCK_COLOR_R = 128, BLOCK_COLOR_G = 128, BLOCK_COLOR_B = 128;

// this is the number of colors found in the function GetColor from colors.c
static const int PLAY_COLORS_COUNT = 10;
void GetColor(unsigned char* color, int index);

#endif //TETRIS_COLORS_H
