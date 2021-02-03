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
static picture tmp_pic;


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* sum_3d_buff:
 *      Sums a source pixel with the specified pixel buffer with given ratio in RGB layers.
 */
static void sum_3d_buff(double pixel_buff[3], picture *src, int src_row, int src_column, frac ratio) {
        pixel_buff[0] += src->arr[src_row][src_column][0] * ratio.num / ratio.denom;
        pixel_buff[1] += src->arr[src_row][src_column][1] * ratio.num / ratio.denom;
        pixel_buff[2] += src->arr[src_row][src_column][2] * ratio.num / ratio.denom;
}
/* push_buff:
 *      Pushes specified buffer on the destination pixel.
 */
static void push_buff(picture *dest, int dest_row, int dest_column, double pixel_buff[3]) {
        dest->arr[dest_row][dest_column][0] = roundl(pixel_buff[0]);
        dest->arr[dest_row][dest_column][1] = roundl(pixel_buff[1]);
        dest->arr[dest_row][dest_column][2] = roundl(pixel_buff[2]);
        pixel_buff[0] = 0;
        pixel_buff[1] = 0;
        pixel_buff[2] = 0;
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

        /* pixel_buff:
         * Before every transfer from old_pic to every new_pic's pixel, we work on
         * pixel_buff.
         * At the end we push pixel_buff on corresponding new_pic's pixel.
         */
        double pixel_buff[3] = {0};
        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek < old_pic->width && newPic_seek < new_pic->width) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        sum_3d_buff(pixel_buff, old_pic, row, oldPic_seek, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        sum_3d_buff(pixel_buff, old_pic, row, oldPic_seek, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        push_buff(new_pic, row, newPic_seek, pixel_buff);
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

        /* pixel_buff:
         * Before every transfer from old_pic to every new_pic's pixel, we work on
         * pixel_buff.
         * At the end we push pixel_buff on corresponding new_pic's pixel.
         */
        double pixel_buff[3] = {0};
        int oldPic_seek = 0, newPic_seek = 0;
        while (oldPic_seek < old_pic->height && newPic_seek < new_pic->height) {
                if (oldPic_share_tmp.num >= newPic_share_tmp.num) {
                        sum_3d_buff(pixel_buff, old_pic, oldPic_seek, column, newPic_share_tmp);
                        oldPic_share_tmp.num -= newPic_share_tmp.num;
                        newPic_share_tmp.num = 0;
                } else {
                        sum_3d_buff(pixel_buff, old_pic, oldPic_seek, column, oldPic_share_tmp);
                        newPic_share_tmp.num -= oldPic_share_tmp.num;
                        oldPic_share_tmp.num = 0;
                }

                if (newPic_share_tmp.num == 0) {
                        push_buff(new_pic, newPic_seek, column, pixel_buff);
                        newPic_seek++;
                        newPic_share_tmp.num = old_pic->height;
                }
                if (oldPic_share_tmp.num == 0) {
                        oldPic_seek++;
                        oldPic_share_tmp.num = new_pic->height;
                }
        }
}

/* make_zero:
 *      This function makes picture array zero.
 */
extern void make_zero(picture *input_pic) {
        for (int i = 0; i < input_pic->height; i++)
                for (int j = 0; j < input_pic->width; j++) {
                        input_pic->arr[i][j][0] = 0;
                        input_pic->arr[i][j][1] = 0;
                        input_pic->arr[i][j][2] = 0;
                }
}
/* make_zero_int:
 *      Like make_zero but for int_picture.
 */
extern void make_zero_int(int_picture *input_pic) {
        for (int i = 0; i < input_pic->height; i++)
                for (int j = 0; j < input_pic->width; j++) {
                        input_pic->arr[i][j][0] = 0;
                        input_pic->arr[i][j][1] = 0;
                        input_pic->arr[i][j][2] = 0;
                }
}


/* The main function of libScale.c
 *
 */
extern void Scale(picture *input_pic, picture *output_pic) {
        tmp_pic.width = output_pic->width;
        tmp_pic.height = input_pic->height;

        for (int i = 0; i < input_pic->height; i++)
                scale_row(&tmp_pic, input_pic, i);
        for (int i = 0; i < tmp_pic.width; i++)
                scale_column(output_pic, &tmp_pic, i);

        make_zero(&tmp_pic);
}
