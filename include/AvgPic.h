/*
 *      Average picture:
 *       Average picture can be obtained from input pictures. But input pictures must be
 *       unmixed and uniform.
 *       For this purpose We crop non-White rows/columns first. And then we scale the picture
 *       to the specified width & height (final_width & final_height global vars).
 *       And then we sum every input picture with an int_picture (is an Structure) and finally we divide
 *       every pixel by number of input pictures to obtain avg_pic.
 *
 */


// Include guard
#ifndef __AVG_PIC__H
#define __AVG_PIC__H


/* Include dependencies
 */
#include <bmpio.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "./AutoCrop.h"
#include "./Scale.h"
#include "./dir_utils.h"


/*********************************************************
 *                   ** STRUCTURES **                    *
 *********************************************************/
/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * four picture globally:
 *      tmp_pic            >> We use this for saving input picture & cropping only non-White rows/columns.
 *      tmp_pic_scale      >> We use this for scaling the tmp_pic using final_width & final_height vars.
 *      tmp_avg_pic        >> This is int_picture ,it's not a char picture. We use this for addition of all
 *                              input pictures (tmp_pic).
 *      avg_pic            >> This is average picture obtained from tmp_avg_pic and number of input pictures.
 */
#include "./shared_structs.h"


/*********************************************************
 *             ** FUNCTION PROTOTYPES **                 *
 *********************************************************/
/* bmp_extension:
 *      returns 1 if file_name has a bmp extension.
 *      returns 0 if not.
 */
// static int bmp_extension(char *file_name) ;

/* check_cache:
 *      This function checks the .cache file. 
 *      and compares its elements with the sign_path files.
 *      if only every file name in sign_path directory was in .cache file then this function 
 *      returns 1, otherwise returns 1.
 */
//static int check_cache(FILE *read_cache,const char *sign_path, int final_width,int final_height);


/* sum_with:
 *      Sums every pixel of from_pic with to_pic.
 */
// static void sum_with(int_picture *to_pic, picture *from_pic) ;

/* finalize_avg:
 *      Divide every pixel of tmp_avg_pic by count and assign avg_pic with this value.
 */
// static void finalize_avg(picture *avg_pic, int_picture *tmp_avg_pic, int count) ;


/* threshold_binary:
 *      Make every pixel whose value is less than a given number totally black (0),
 *      otherwise make it totally white (255).
 */
extern void threshold_binary(picture *a, int value);

/* poverty_line:
 *      This function finds a number [0,255] that n percent of given picture's pixels 
 *      are below that number.
 */
extern unsigned int poverty_line(picture *a, unsigned int percent); 


/* AvgPic:
 *      Wants a picture directory.
 *      This function does the main job for us.
 *      This function finds every .bmp file (for this purpose uses dirent.h) in sign_path dir.
 *      This function uses a very simple cache system , so if you call this function twice the running
 *      time is less.
 *
 *      At the end creates avg.bmp and avg_thr.bmp 
 */
extern int AvgPic(const char *sign_path,int final_width,int final_height,const char *working_dir);

#endif
