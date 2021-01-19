/*
 *      AutoCrop
 *      
 *      Steps:
 *      1. nonWhite_columns_ranges/nonWhite_rows_ranges to find pictures
 *      2. remove_white_lines for every found picture
 *      3. crop_then_saveBMP every picture
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bmpio.h>


/*              ** STRUCTURES **                */

/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * two picture globally:
 *      pic     >> we use this object to save our main picture
 *      tmp_pic >> we use this object temporary for cropping purposes
 */
#include "./shared_structs.c"
static picture pic, tmp_pic;

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


/*              ** FUNCTION DEFINITIONS **              */

/* is_column_white / is_row_white:
 *      If a column/row from a number to a number was white
 *      returns 1 otherwise 0
 */
static int is_column_white(int x, int from, int to) {
        for (int i = from; i < to; i++) {
                if (pic.arr[i][x][0] != 255 || pic.arr[i][x][1] != 255 || pic.arr[i][x][2] != 255)
                        return 0;
        }
        return 1;
}
static int is_row_white(int y, int from, int to) {
        for (int i = from; i < to; i++) {
                if (pic.arr[y][i][0] != 255 || pic.arr[y][i][1] != 255 || pic.arr[y][i][2] != 255)
                        return 0;
        }
        return 1;
}


/* columns_ranges/rows_ranges:
 *      returns array of ranges that include non-white columns/rows
 *      This kind of function use malloc_arr structure
 */
static malloc_arr *nonWhite_columns_ranges() {
        malloc_arr *ranges;
        ranges = malloc(sizeof(malloc_arr));
        ranges->sizeof_arr = 0;
        ranges->arr_po = (int *) malloc(sizeof(int));

        int i = 0, last_column = 1, current_column;
        while (i < pic.width) {
                current_column = is_column_white(i, 0, pic.height);
                if (current_column + last_column == 1) {
                        ranges->sizeof_arr += 2;

                        ranges->arr_po = (int *) realloc(ranges->arr_po, sizeof(int) * ranges->sizeof_arr);

                        ranges->arr_po[ranges->sizeof_arr - 2] = i;
                        while (current_column == 0) {
                                i++;
                                current_column = is_column_white(i, 0, pic.height);
                        }
                        ranges->arr_po[ranges->sizeof_arr - 1] = i;
                }
                i++;
        }

        return ranges;
}
static malloc_arr *nonWhite_rows_ranges() {
        malloc_arr *ranges;
        ranges = malloc(sizeof(malloc_arr));
        ranges->sizeof_arr = 0;
        ranges->arr_po = (int *) malloc(sizeof(int));

        int i = 0, last_row = 1, current_row;
        while (i < pic.height) {
                current_row = is_row_white(i, 0, pic.width);
                if (current_row + last_row == 1) {
                        ranges->sizeof_arr += 2;

                        ranges->arr_po = (int *) realloc(ranges->arr_po, sizeof(int) * ranges->sizeof_arr);

                        ranges->arr_po[ranges->sizeof_arr - 2] = i;
                        while (current_row == 0) {
                                i++;
                                current_row = is_row_white(i, 0, pic.width);
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
static square remove_white_lines(square *a) {
        int i;
        square b;
        // from Right to Left
        for (i = a->right; i >= a->left; i--) {
                if (!is_column_white(i, a->top, a->bottom)) {
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
                if (!is_column_white(i, a->top, a->bottom)) {
                        b.left = i;
                        break;
                }
        }
        // from Top to Bottom
        for (i = a->top; i <= a->bottom; i++) {
                if (!is_row_white(i, a->left, a->right)) {
                        b.top = i;
                        break;
                }
        }
        // from Bottom to Top
        for (i = a->bottom; i >= a->top; i--) {
                if (!is_row_white(i, a->left, a->right)) {
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
static void crop_then_saveBMP(square *crop, char *output_path) {
        tmp_pic.width = crop->right - crop->left + 1;
        tmp_pic.height = crop->bottom - crop->top + 1;

        for (int i = crop->top; i <= crop->bottom; i++) {
                for (int j = crop->left; j <= crop->right; j++) {
                        tmp_pic.arr[i - crop->top][j - crop->left][0] = pic.arr[i][j][0];
                        tmp_pic.arr[i - crop->top][j - crop->left][1] = pic.arr[i][j][1];
                        tmp_pic.arr[i - crop->top][j - crop->left][2] = pic.arr[i][j][2];
                }
        }

        saveBMP(tmp_pic.arr, tmp_pic.width, tmp_pic.height, output_path);
}

/* The main function of AutoCrop.c
 *
 */
void AutoCrop(char *input_path) {
        char path[100];
        strcpy(path , input_path);

        readBMP(path, &pic.width, &pic.height, pic.arr);

        malloc_arr *x;
        x = nonWhite_columns_ranges();

        malloc_arr *y;
        y = nonWhite_rows_ranges();

        square crop;
        for (int i = 0; i < x->sizeof_arr; i += 2) {
                for (int j = 0; j < y->sizeof_arr; j += 2) {
                        crop.left = x->arr_po[i];
                        crop.right = x->arr_po[i + 1];
                        crop.top = y->arr_po[j];
                        crop.bottom = y->arr_po[j + 1];

                        crop = remove_white_lines(&crop);
                        if (crop.right != -1) { // If square was not totally white
                                sprintf(path, "%d_%d.bmp",j/2,i/2);
                                crop_then_saveBMP(&crop, path);
                        }
                }
        }
}

