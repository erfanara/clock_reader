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


/* All of Structures and Include dependencies are in Scale.h
 */
#include "../include/Scale.h"


/*********************************************************
 *                  ** GLOBAL VARS **                    *
 *********************************************************/
static picture oldPic, newPic_row, newPic_row_column;


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* cp_3d:
 *      Copies a source pixel to the destination pixel with given ratio in RGB layers.
 */
static void cp_3d(picture *dest, int dest_row, int dest_column, picture *src, int src_row, int src_column,
                  frac ratio) {
        dest->arr[dest_row][dest_column][0] += src->arr[src_row][src_column][0] * ratio.num / ratio.denom;
        dest->arr[dest_row][dest_column][1] += src->arr[src_row][src_column][1] * ratio.num / ratio.denom;
        dest->arr[dest_row][dest_column][2] += src->arr[src_row][src_column][2] * ratio.num / ratio.denom;
}

/* scale_row:
 *      scales given row from old picture to the new picture.
 */
static void scale_row(picture *new_pic, picture *old_pic, int row) {
        /* oldPic_share:
         * Share of every pixel of old pic.
         */
        frac oldPic_share_tmp = {.num = new_pic->width, .denom = old_pic->width};
        /* newPic_share:
         * Share of every pixel of new pic.
         * (newPic_share = 1)
         */
        frac newPic_share_tmp = {.num = old_pic->width, .denom = old_pic->width};

        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek <= old_pic->width && newPic_seek <= new_pic->width) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        cp_3d(new_pic, row, newPic_seek, old_pic, row, oldPic_seek, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        cp_3d(new_pic, row, newPic_seek, old_pic, row, oldPic_seek, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        newPic_seek++;
                        newPic_share_tmp.num = old_pic->width;
                }
                if (oldPic_share_tmp.num == 0) {
                        oldPic_seek++;
                        oldPic_share_tmp.num = new_pic->width;
                }
        }
}

/* scale_column:
 *      scales given column from old picture to the new picture.
 */
static void scale_column(picture *new_pic, picture *old_pic, int column) {
        /* oldPic_share:
         * Share of every pixel of old pic.
         */
        frac oldPic_share_tmp = {.num = new_pic->height, .denom = old_pic->height};
        /* newPic_share:
         * Share of every pixel of new pic.
         * (newPic_share = 1)
         */
        frac newPic_share_tmp = {.num = old_pic->height, .denom = old_pic->height};

        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek <= old_pic->height && newPic_seek <= new_pic->height) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        cp_3d(new_pic, newPic_seek, column, old_pic, oldPic_seek, column, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        cp_3d(new_pic, newPic_seek, column, old_pic, oldPic_seek, column, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        newPic_seek++;
                        newPic_share_tmp.num = old_pic->height;
                }
                if (oldPic_share_tmp.num == 0) {
                        oldPic_seek++;
                        oldPic_share_tmp.num = new_pic->height;
                }
        }
}

/* The main function of scale.c
 *
 */
extern void Scale(char *input_path, int input_width, int input_height, char *output_name) {
        char path[100];
        strcpy(path, input_path);

        newPic_row_column.width = input_width;
        newPic_row_column.height = input_height;

        newPic_row.width = newPic_row_column.width;

        readBMP(path, &oldPic.width, &oldPic.height, oldPic.arr);
        newPic_row.height = oldPic.height;

        for (int i = 0; i < oldPic.height; i++)
                scale_row(&newPic_row, &oldPic, i);
        for (int i = 0; i < newPic_row.width; i++)
                scale_column(&newPic_row_column, &newPic_row, i);

        saveBMP(newPic_row_column.arr, newPic_row_column.width, newPic_row_column.height, output_name);
}
