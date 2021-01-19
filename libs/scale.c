// Mohammad Erfan Arasteh 9912762427

/*
 *      Scale  
 *
 *      Steps:
 *      This program works using Nearest-neighbor interpolation algorithm.
 *      This program calculates value of every pixel by calculating
 *      the shares of pixels of given picture.
 *      Firstly we scale the given picture in rows, and then we scale it in
 *      columns.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <bmpio.h>
#include "shared_structs.c"

/*              ** STRUCTURES **                */

picture oldPic, newPic_row, newPic_row_column;

/* Every fraction consists of a numerator and a denominator
 */
typedef struct fraction {
        int num;
        int denom;
} frac;

/*              ** FUNCTION PROTOTYPES **               */

/* cp_3d:
 *      Copies a source pixel to the destination pixel with given ratio in RGB layers.
 */
static void cp_3d(picture *dest, int dest_row, int dest_column, picture *src, int src_row, int src_column,
           frac ratio);
/* scale_row:
 *      scales given row from old picture to the new picture.
 */
static void scale_row(picture *new, picture *old, int row);
/* scale_column:
 *      scales given column from old picture to the new picture.
 */
static void scale_column(picture *new, picture *old, int column);

/*              ** MAIN **              */

int main() {
        char path[100];
        scanf("%100s", path);
        scanf("%d%d", &newPic_row_column.width, &newPic_row_column.height);
        newPic_row.width = newPic_row_column.width;

        readBMP(path, &oldPic.width, &oldPic.height, oldPic.arr);
        newPic_row.height = oldPic.height;

        for (int i = 0; i < oldPic.height; i++)
                scale_row(&newPic_row, &oldPic, i);
        for (int i = 0; i < newPic_row.width; i++)
                scale_column(&newPic_row_column, &newPic_row, i);

        saveBMP(newPic_row_column.arr, newPic_row_column.width, newPic_row_column.height, "O_scaled.bmp");
}

/*              ** FUNCTION DEFINITIONS **              */

static void cp_3d(picture *dest, int dest_row, int dest_column, picture *src, int src_row, int src_column,
           frac ratio) {
        dest->arr[dest_row][dest_column][0] += src->arr[src_row][src_column][0] * ratio.num / ratio.denom;
        dest->arr[dest_row][dest_column][1] += src->arr[src_row][src_column][1] * ratio.num / ratio.denom;
        dest->arr[dest_row][dest_column][2] += src->arr[src_row][src_column][2] * ratio.num / ratio.denom;
}

static void scale_row(picture *new, picture *old, int row) {
        /* oldPic_share:
         * Share of every pixel of old pic.
         */
        frac oldPic_share_tmp = {.num = new->width, .denom = old->width};
        /* newPic_share:
         * Share of every pixel of new pic.
         * (newPic_share = 1)
         */
        frac newPic_share_tmp = {.num = old->width, .denom = old->width};

        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek <= old->width && newPic_seek <= new->width) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        cp_3d(new, row, newPic_seek, old, row, oldPic_seek, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        cp_3d(new, row, newPic_seek, old, row, oldPic_seek, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        newPic_seek++;
                        newPic_share_tmp.num = old->width;
                }
                if (oldPic_share_tmp.num == 0) {
                        oldPic_seek++;
                        oldPic_share_tmp.num = new->width;
                }
        }
}

static void scale_column(picture *new, picture *old, int column) {
        /* oldPic_share:
         * Share of every pixel of old pic.
         */
        frac oldPic_share_tmp = {.num = new->height, .denom = old->height};
        /* newPic_share:
         * Share of every pixel of new pic.
         * (newPic_share = 1)
         */
        frac newPic_share_tmp = {.num = old->height, .denom = old->height};

        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek <= old->height && newPic_seek <= new->height) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        cp_3d(new, newPic_seek, column, old, oldPic_seek, column, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        cp_3d(new, newPic_seek, column, old, oldPic_seek, column, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        newPic_seek++;
                        newPic_share_tmp.num = old->height;
                }
                if (oldPic_share_tmp.num == 0) {
                        oldPic_seek++;
                        oldPic_share_tmp.num = new->height;
                }
        }
}
