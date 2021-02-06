/*
 *      AutoCrop
 *
 *      Steps:
 *      1. nonWhite_columns_ranges/nonWhite_rows_ranges to find pictures
 *      2. remove_white_lines for every found picture
 *      3. crop_then_saveBMP every picture
 *
 */


/* All of Structures and Include dependencies are in AutoCrop.h
 */
#include "../include/AutoCrop.h"


/*********************************************************
 *                  ** GLOBAL VARS **                    *
 *********************************************************/
static picture pic, tmp_pic;


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* is_column_white / is_row_white:
 *      If a column/row from a number to a number was white
 *      returns 1 otherwise 0
 */
static int is_column_white(picture *input_pic, int x, int from, int to) {
        for (int i = from; i < to; i++) {
                if (input_pic->arr[i][x][0] != 255 || input_pic->arr[i][x][1] != 255 ||
                    input_pic->arr[i][x][2] != 255)
                        return 0;
        }
        return 1;
}
static int is_row_white(picture *input_pic, int y, int from, int to) {
        for (int i = from; i < to; i++) {
                if (input_pic->arr[y][i][0] != 255 || input_pic->arr[y][i][1] != 255 ||
                    input_pic->arr[y][i][2] != 255)
                        return 0;
        }
        return 1;
}


/* columns_ranges/rows_ranges:
 *      returns array of ranges that include non-white columns/rows
 *      This kind of function use malloc_arr structure
 */
static malloc_arr *nonWhite_columns_ranges(picture *input_pic) {
        malloc_arr *ranges;
        ranges = malloc(sizeof(malloc_arr));
        ranges->sizeof_arr = 0;
        ranges->arr_po = (int *)malloc(sizeof(int));

        int i = 0, last_column = 1, current_column;
        while (i < input_pic->width) {
                current_column = is_column_white(input_pic, i, 0, input_pic->height);
                if (current_column + last_column == 1) {
                        ranges->sizeof_arr += 2;

                        ranges->arr_po = (int *)realloc(ranges->arr_po, sizeof(int) * ranges->sizeof_arr);

                        ranges->arr_po[ranges->sizeof_arr - 2] = i;
                        while (current_column == 0) {
                                i++;
                                current_column = is_column_white(input_pic, i, 0, input_pic->height);
                        }
                        ranges->arr_po[ranges->sizeof_arr - 1] = i;
                }
                i++;
        }

        return ranges;
}
static malloc_arr *nonWhite_rows_ranges(picture *input_pic) {
        malloc_arr *ranges;
        ranges = malloc(sizeof(malloc_arr));
        ranges->sizeof_arr = 0;
        ranges->arr_po = (int *)malloc(sizeof(int));

        int i = 0, last_row = 1, current_row;
        while (i < input_pic->height) {
                current_row = is_row_white(input_pic, i, 0, input_pic->width);
                if (current_row + last_row == 1) {
                        ranges->sizeof_arr += 2;

                        ranges->arr_po = (int *)realloc(ranges->arr_po, sizeof(int) * ranges->sizeof_arr);

                        ranges->arr_po[ranges->sizeof_arr - 2] = i;
                        while (current_row == 0) {
                                i++;
                                current_row = is_row_white(input_pic, i, 0, input_pic->width);
                        }
                        ranges->arr_po[ranges->sizeof_arr - 1] = i;
                }
                i++;
        }

        return ranges;
}

/* remove_white_lines:
 *      Wants an square , then finds non-white columns/rows from that square.
 *      Then returns new square.
 *      This function uses square structure.
 */
static square remove_white_lines(picture *input_pic, square *a) {
        int i;
        square b;
        // from Right to Left
        for (i = a->right; i >= a->left; i--) {
                if (!is_column_white(input_pic, i, a->top, a->bottom)) {
                        b.right = i;
                        break;
                }
        }
        if (i == a->left - 1) { // If square is totally white
                b.right = -1;
                return b;
        }
        // form Left to Right
        for (i = a->left; i <= a->right; i++) {
                if (!is_column_white(input_pic, i, a->top, a->bottom)) {
                        b.left = i;
                        break;
                }
        }
        // from Top to Bottom
        for (i = a->top; i <= a->bottom; i++) {
                if (!is_row_white(input_pic, i, a->left, a->right)) {
                        b.top = i;
                        break;
                }
        }
        // from Bottom to Top
        for (i = a->bottom; i >= a->top; i--) {
                if (!is_row_white(input_pic, i, a->left, a->right)) {
                        b.bottom = i;
                        break;
                }
        }
        return b;
}

/* crop_then_saveBMP:
 *      Wants an square for cropping.
 *      After cropping saves that square using saveBMP function.
 */
static void crop_the_square(picture *input_pic, square *sq, picture *output_pic) {
        output_pic->width = sq->right - sq->left + 1;
        output_pic->height = sq->bottom - sq->top + 1;

        for (int i = sq->top; i <= sq->bottom; i++) {
                for (int j = sq->left; j <= sq->right; j++) {
                        output_pic->arr[i - sq->top][j - sq->left][0] = input_pic->arr[i][j][0];
                        output_pic->arr[i - sq->top][j - sq->left][1] = input_pic->arr[i][j][1];
                        output_pic->arr[i - sq->top][j - sq->left][2] = input_pic->arr[i][j][2];
                }
        }
}


/* The main function of AutoCrop.c
 * This function does the job. Crops pictures in a picture.
 * This function wants output_directory for saving the output pictures.
 * At the end returns number of pictures cropped.
 */
extern int AutoCrop(char *pic_path, char *output_dir) {
        int cropped=0;
        char path[100];
        strcpy(path, pic_path);

        readBMP(path, &pic.width, &pic.height, pic.arr);

        malloc_arr *x;
        x = nonWhite_columns_ranges(&pic);

        malloc_arr *y;
        y = nonWhite_rows_ranges(&pic);

        square crop;
        for (int i = 0; i < x->sizeof_arr; i += 2) {
                for (int j = 0; j < y->sizeof_arr; j += 2) {
                        crop.left = x->arr_po[i];
                        crop.right = x->arr_po[i + 1];
                        crop.top = y->arr_po[j];
                        crop.bottom = y->arr_po[j + 1];

                        crop = remove_white_lines(&pic, &crop);
                        if (crop.right != -1) { // If square was not totally white
                                cropped++;
                                sprintf(path, "%s/%d_%d.bmp", output_dir, j / 2, i / 2);
                                crop_the_square(&pic, &crop, &tmp_pic);
                                saveBMP(tmp_pic.arr, tmp_pic.width, tmp_pic.height, path);
                        }
                }
        }
        free(x->arr_po);
        free(y->arr_po);

        return cropped;
}

/* If input picture was a single picture , then we need this function
 * to remove the white lines.
 */
extern void SingleCrop(picture *input_pic, picture *output_pic) {
        square crop;
        crop.left = 0;
        crop.right = input_pic->width - 1;
        crop.top = 0;
        crop.bottom = input_pic->height - 1;

        crop = remove_white_lines(input_pic, &crop);
        if (crop.right != -1) { // If square was not totally white
                crop_the_square(input_pic, &crop, output_pic);
        }
}
