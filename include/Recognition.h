/*      
 *
 *      Recognition:
 *       When we have avg pictures, then we can compare a sample with avg pictures.
 *
 */


// Include guard
#ifndef __RECOGNITION__H
#define __RECOGNITION__H


/* Include dependencies
 */
#include "./AutoCrop.h"
#include "./AvgPic.h"
#include "./Scale.h"
#include <bmpio.h>
#include <stdio.h>
#include <string.h>


/*********************************************************
 *                   ** STRUCTURES **                    *
 *********************************************************/
/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * three picture globally:
 *      tmp_pic            >> We use this for saving input picture.
 *      output_pic         >> We use this for saving scaled input picture.
 *      avg_pic            >> This is average picture collected from cache files.
 */
#include "./shared_structs.h"


/*********************************************************
 *             ** FUNCTION PROTOTYPES **                 *
 *********************************************************/
/* diff_pix:
 *      This function finds the difference between two corresponding pixel in two image.
 */
//static unsigned int diff_pix(picture *a, picture *b, int row, int column);

/* pix_matches:
 *      This function wants two picture with the same size.
 *      This function counts every diff_pix less than n for every corresponding pixels.
 */
//static unsigned int pix_matches(picture *a, picture *b, unsigned int n);

/* Recognition_pix:
 *      This function returns recognized number (best match).
 *      This function works using pix_matches function. 
 *      Width/height of Cache pictures should be given.
 */
extern unsigned int Recognition_pix(char *pic_path,const char *DataSets_path,int width,int height);


#endif
