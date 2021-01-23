/*
 *      AutoCrop
 *
 *      Steps:
 *      1. nonWhite_columns_ranges/nonWhite_rows_ranges to find pictures
 *      2. remove_white_lines for every found picture
 *      3. crop_then_saveBMP every picture
 *
 */

//Include guard
#ifndef __AUTOCROP__H
#define __AUTOCROP__H


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
 * two picture globally:
 *      pic     >> we use this object to save our main picture
 *      tmp_pic >> we use this object temporary for cropping purposes
 */
#include "./shared_structs.h"

/* When we reserve memory using malloc ,then we need to remember it's
 * size . So structures are suitable for this purpose.
 */
typedef struct malloc_array {
        size_t sizeof_arr;
        int *arr_po;
} malloc_arr;

/* If you want to crop a part of picture you need to save
 * cropping parameters . So structures are suitable for this purpose.
 */
typedef struct square_frame {
        int top;
        int bottom;
        int left;
        int right;
} square;


/*********************************************************
 *             ** FUNCTION PROTOTYPES **                 *
 *********************************************************/
/* is_column_white / is_row_white:
 *      If a column/row from a number to a number was white
 *      returns 1 otherwise 0
 */
//static int is_column_white(int x, int from, int to);
//static int is_row_white(int y, int from, int to);


/* columns_ranges/rows_ranges:
 *      returns array of ranges that include non-white columns/rows
 *      This kind of function use malloc_arr structure
 */
//static malloc_arr *nonWhite_columns_ranges();
//static malloc_arr *nonWhite_rows_ranges();


/* remove_white_lines:
 *      Wants an square , then finds non-white columns/rows from that square.
 *      Then returns new square.
 *      This function uses square structure.
 */
//static square remove_white_lines(square *a);


/* crop_then_saveBMP:
 *      Wants an square for cropping.
 *      After cropping saves that square using saveBMP function.
 */
//static void crop_then_saveBMP(square *crop, char *output_path);


/* The main function of AutoCrop.c
 * This function does the job. Crops pictures in a picture.
 * This function wants output_directory for saving the output pictures.
 * At the end returns number of pictures cropped.
 */
extern int AutoCrop(char *pic_path,char *output_dir);

/* If input picture was a single picture , then we need this function
 * to remove the white lines.
 */
extern void SingleCrop(picture *input_pic,picture *output_pic);

#endif
