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


// Include guard
#ifndef __SCALE__H
#define __SCALE__H


/* Include dependencies
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bmpio.h>


/*********************************************************
 *                   ** STRUCTURES **                    *
 *********************************************************/
/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * three picture globally:
 *      oldPic            >> We use this for saving input picture
 *      newPic_row        >> We use this for scaling the oldPic in rows
 *      newPic_row_column >> We use this for scaling thw newPic_row in columns
 */
#include "./shared_structs.h"

/* Every fraction consists of a numerator and a denominator
 */
typedef struct fraction {
        int num;
        int denom;
} frac;


/*********************************************************
 *             ** FUNCTION PROTOTYPES **                 *
 *********************************************************/
/* sum_3d:
 *      Copies a source pixel to the destination pixel with given ratio in RGB layers.
 */
//static void sum_3d(picture *dest, int dest_row, int dest_column, picture *src, int src_row, int src_column,
//                  frac ratio);

/* scale_row:
 *      scales given row from old picture to the new picture.
 */
//static void scale_row(picture *new_pic, picture *old_pic, int row);

/* scale_column:
 *      scales given column from old picture to the new picture.
 */
//static void scale_column(picture *new_pic, picture *old_pic, int column);


/* make_zero:
 *      This function makes picture array zero.
 */
extern void make_zero(picture *input_pic);

/* The main function of scale.c
 *
 */
extern void Scale(picture *input_pic, picture *output_pic);

#endif
