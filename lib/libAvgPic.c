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


/* All of Structures and Include dependencies are in AvgPic.h
 */
#include "../include/AvgPic.h"


/*********************************************************
 *                  ** GLOBAL VARS **                    *
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
static picture tmp_pic, tmp_pic_scale, avg_pic;
static int_picture tmp_avg_pic;
unsigned int final_width = 300;
unsigned int final_height = 600;


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* bmp_extension:
 *      returns 1 if file_name has a bmp extension.
 *      returns 0 if not.
 */
static int bmp_extension(char *file_name) {
        unsigned int len = 0;
        for (; file_name[len] != '\0'; len++)
                ;
        if (file_name[len - 1] == 'p')
                if (file_name[len - 2] == 'm')
                        if (file_name[len - 3] == 'b')
                                if (file_name[len - 4] == '.')
                                        return 1;
        return 0;
}

/* sum_with:
 *      Sums every pixel of from_pic with to_pic.
 */
static void sum_with(int_picture *to_pic, picture *from_pic) {
        for (int i = 0; i < to_pic->height; i++)
                for (int j = 0; j < to_pic->width; j++) {
                        to_pic->arr[i][j][0] += from_pic->arr[i][j][0];
                        to_pic->arr[i][j][1] += from_pic->arr[i][j][1];
                        to_pic->arr[i][j][2] += from_pic->arr[i][j][2];
                }
}

/* finalize_avg:
 *      Divide every pixel of tmp_avg_pic by count and assign avg_pic with this value.
 */
static void finalize_avg(picture *avg_pic, int_picture *tmp_avg_pic, int count) {
        for (int i = 0; i < avg_pic->height; i++)
                for (int j = 0; j < avg_pic->width; j++) {
                        avg_pic->arr[i][j][0] = tmp_avg_pic->arr[i][j][0] / count;
                        avg_pic->arr[i][j][1] = tmp_avg_pic->arr[i][j][1] / count;
                        avg_pic->arr[i][j][2] = tmp_avg_pic->arr[i][j][2] / count;
                }
}


/* AvgPic:
 *      Wants a picture directory.
 *      This function does the main job for us.
 *      This function finds every .bmp files except avg.bmp and O_X.bmp files.(for this purpose uses
 * dirent.h) At the end creates avg.
 *      bmp returns 0 if operation was successful.
 */
extern int AvgPic(char *path) {
        DIR *dir_p;
        struct dirent *entry;
        dir_p = opendir(path);
        if (dir_p == NULL) {
                perror("opendir_average_func");
                return -1;
        }

        unsigned int count = 0;

        /* Our specified width/height
         */
        avg_pic.width = final_width;
        avg_pic.height = final_height;
        tmp_avg_pic.width = final_width;
        tmp_avg_pic.height = final_height;
        tmp_pic_scale.width = final_width;
        tmp_pic_scale.height = final_height;

        char tmp_path[512];
        while ((entry = readdir(dir_p)))
                if (bmp_extension(entry->d_name) && strcmp(entry->d_name, "avg.bmp") != 0 &&
                    entry->d_name[0] != 'O') {
                        count++;
                        sprintf(tmp_path, "%s/%s", path, entry->d_name);
                        readBMP(tmp_path, &tmp_pic.width, &tmp_pic.height, tmp_pic.arr);

                        SingleCrop(&tmp_pic, &tmp_pic);

                        Scale(&tmp_pic, &tmp_pic_scale);

                        sum_with(&tmp_avg_pic, &tmp_pic_scale);

                        sprintf(tmp_path, "%s/O_%s", path, entry->d_name);
                        saveBMP(tmp_pic_scale.arr, tmp_pic_scale.width, tmp_pic_scale.height, tmp_path);
                        make_zero(&tmp_pic_scale); // make all elements of array zero
                }

        finalize_avg(&avg_pic, &tmp_avg_pic, count);

        sprintf(tmp_path, "%s/avg.bmp", path);
        saveBMP(avg_pic.arr, avg_pic.width, avg_pic.height, tmp_path);
        make_zero(&avg_pic);
        make_zero_int(&tmp_avg_pic);

        closedir(dir_p);
        return 0;
}
