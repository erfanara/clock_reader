/*      
 *
 *      Recognition:
 *       When we have avg pictures, then we can compare a sample with avg pictures.
 *
 */


/* All of Structures and Include dependencies are in Recognition.h
 */
#include "../include/Recognition.h"


/*********************************************************
 *                  ** GLOBAL VARS **                    *
 *********************************************************/
/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * three picture globally:
 *      tmp_pic            >> We use this for saving input picture.
 *      output_pic         >> We use this for saving scaled input picture.
 *      avg_pic            >> This is average picture collected from cache files.
 */
static picture tmp_pic,output_pic,avg_pic;


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* diff_pix:
 *      This function finds the difference between two corresponding pixel in two image.
 */
static unsigned int diff_pix(picture *a, picture *b, int row, int column) {
        return (abs(a->arr[row][column][0] - b->arr[row][column][0]) +
                abs(a->arr[row][column][1] - b->arr[row][column][1]) +
                abs(a->arr[row][column][2] - b->arr[row][column][2])) /
               3;
}

/* pix_matches:
 *      This function wants two picture with the same size.
 *      This function counts every diff_pix less than n for every corresponding pixels.
 */
static unsigned int pix_matches(picture *a,picture *b,unsigned int n){
        unsigned int count=0;
        for (int i = 0; i < a->height; i++) {
                for (int j = 0; j < a->width; j++) {
                        if(diff_pix(a,b,i,j) <= n)
                                count++;
                }
        }
        return count;
}

/* Recognition_pix:
 *      This function returns recognized number (best match).
 *      This function works using pix_matches function. 
 *      Width/height of Cache pictures should be given.
 */
extern unsigned int Recognition_pix(char *pic_path, const char *DataSets_path,int width,int height) {
        char tmp_path[255];
        strcpy(tmp_path, pic_path);

        readBMP(tmp_path, &tmp_pic.width, &tmp_pic.height, tmp_pic.arr);
        output_pic.width = width;
        output_pic.height = height;
        Scale(&tmp_pic, &output_pic);
        threshold_binary(&output_pic,poverty_line(&output_pic,63));

        unsigned int max = 0, tmp_diff;
        int k;
        for (int i = 0; i <= 9; i++) {
                sprintf(tmp_path, "%s/%d/avg_thr.bmp", DataSets_path, i);
                readBMP(tmp_path, &avg_pic.width, &avg_pic.height, avg_pic.arr);
                tmp_diff = pix_matches(&avg_pic , &output_pic , 0 );
                //printf("  %u  \n", tmp_diff);
                if (max < tmp_diff) {
                        k = i;
                        max = tmp_diff;
                }
        }
        return k;
}

