//
// Created by wortelus on 23.11.21.
//

#include "colors.h"

void GetColor(unsigned char* color, int index) {
    index %= PLAY_COLORS_COUNT;
    switch (index) {
        case 0:
            color[0] = 128;
            color[1] = 128;
            color[2] = 128;
            break;
        case 1:
            color[0] = 255;
            color[1] = 0;
            color[2] = 0;
            break;
        case 2:
            color[0] = 0;
            color[1] = 255;
            color[2] = 0;
            break;
        case 3:
            color[0] = 0;
            color[1] = 0;
            color[2] = 255;
            break;
        case 4:
            color[0] = 255;
            color[1] = 0;
            color[2] = 255;
            break;
        case 5:
            color[0] = 0;
            color[1] = 255;
            color[2] = 255;
            break;
        case 6:
            color[0] = 255;
            color[1] = 128;
            color[2] = 128;
            break;
        case 7:
            color[0] = 128;
            color[1] = 255;
            color[2] = 128;
            break;
        case 8:
            color[0] = 128;
            color[1] = 128;
            color[2] = 255;
            break;
        case 9:
            color[0] = 255;
            color[1] = 128;
            color[2] = 255;
            break;
        case 10:
            color[0] = 128;
            color[1] = 255;
            color[2] = 255;
            break;
        default:
            color[0] = 255;
            color[1] = 255;
            color[2] = 255;
            break;

    }
}