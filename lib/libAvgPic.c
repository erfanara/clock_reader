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


/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
/* bmp_extension:
 *      returns 1 if file_name has a bmp extension.
 *      returns 0 if not.
 */
static int bmp_extension(char *file_name) {
        unsigned int len = strlen(file_name);
        if (file_name[len - 1] == 'p')
                if (file_name[len - 2] == 'm')
                        if (file_name[len - 3] == 'b')
                                if (file_name[len - 4] == '.')
                                        return 1;
        return 0;
}

/* check_cache:
 *      This function checks the .cache file.
 *      and compares its elements with the sign_path files.
 *      if only every file name in sign_path directory was in .cache file then this function
 *      returns 1, otherwise returns 1.
 */
static int check_cache(FILE *read_cache, const char *sign_path, int final_width, int final_height) {
        // Firstly check the final_width and final_height in cache file
        int cache_width, cache_height;
        fscanf(read_cache, "%d %d\n", &cache_width, &cache_height);
        if (cache_width != final_width || cache_height != final_height)
                return 0;

        // Get ready for listing the sign_path directory
        DIR *dir_p;
        struct dirent *entry;
        dir_p = opendir(sign_path);
        if (dir_p == NULL) {
                perror("opendir_error_in_check_cache");
                return -1;
        }

        char name_in_cache[100];
        int is_exist = 0;
        while ((entry = readdir(dir_p)))
                if (bmp_extension(entry->d_name)) {
                        while (fscanf(read_cache, "%s\n", name_in_cache) != EOF)
                                if (strcmp(entry->d_name, name_in_cache) == 0)
                                        is_exist = 1;


                        if (is_exist == 0)
                                return 0;

                        rewind(read_cache);
                        is_exist = 0;
                }

        fclose(read_cache);
        // cache seems to be ok , so return 1
        return 1;
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

/* threshold_binary:
 *      Make every pixel whose value is less than a given number totally black (0),
 *      otherwise make it totally white (255).
 */
extern void threshold_binary(picture *a, int value) {
        for (int i = 0; i < a->height; i++)
                for (int j = 0; j < a->width; j++) {
                        if (a->arr[i][j][0] < value || a->arr[i][j][1] < value || a->arr[i][j][2] < value) {
                                a->arr[i][j][0] = 0;
                                a->arr[i][j][1] = 0;
                                a->arr[i][j][2] = 0;
                        } else {
                                a->arr[i][j][0] = 255;
                                a->arr[i][j][1] = 255;
                                a->arr[i][j][2] = 255;
                        }
                }
}

/* poverty_line:
 *      This function finds a number [0,255] that n percent of given picture's pixels
 *      are below that number.
 */
extern unsigned int poverty_line(picture *a, unsigned int percent) {
        unsigned int pix_map_count[256] = {0}, total = 0, i;
        unsigned char pix_tmp;
        for (i = 0; i < a->height; i++) {
                for (int j = 0; j < a->width; j++) {
                        pix_tmp = (a->arr[i][j][0] + a->arr[i][j][1] + a->arr[i][j][2]) / 3;
                        if (pix_tmp < 253) {
                                pix_map_count[pix_tmp]++;
                                total++;
                        }
                }
        }

        percent = total * percent / 100;

        i = 0;
        for (int x = 0; x <= percent; i++) {
                x += pix_map_count[i];
        }
        return i;
}

/* AvgPic:
 *      Wants a picture directory.
 *      This function does the main job for us.
 *      This function finds every .bmp file (for this purpose uses dirent.h) in sign_path dir.
 *      This function uses a very simple cache system , so if you call this function twice the running
 *      time is less.
 *
 *      At the end creates avg.bmp and avg_thr.bmp
 */
extern int AvgPic(const char *sign_path, int final_width, int final_height, const char *working_dir) {
        char tmp_path[300], cache_path[256], sign;
        sign = sign_path[strlen(sign_path) - 1];

        // create working directory if not exist
        make_dir(working_dir);

        // create sign_path directory if not exist
        sprintf(tmp_path, "%s/%c", working_dir, sign);
        make_dir(tmp_path);

        // create .cache directory in working_dir if not exist
        sprintf(tmp_path, "%s/.cache", working_dir);
        make_dir(tmp_path);

        // Get ready for listing the sign_path directory
        DIR *dir_p;
        struct dirent *entry;
        dir_p = opendir(sign_path);
        if (dir_p == NULL) {
                perror("opendir_error_in_average_func, maybe the DataSet path is wrong");
                return 0;
        }

        // Get ready for checking the cache file
        FILE *read_cache;
        sprintf(cache_path, "%s/.cache/%c.cache", working_dir, sign);
        read_cache = fopen(cache_path, "r");

        if (read_cache == NULL || !check_cache(read_cache, sign_path,final_width,final_height)) {
                FILE *write_cache;
                write_cache = fopen(cache_path, "w");
                fprintf(write_cache, "%d %d\n", final_width, final_height);

                /* Our specified width/height for scaling input pictures
                 */
                avg_pic.width = final_width;
                avg_pic.height = final_height;
                tmp_avg_pic.width = final_width;
                tmp_avg_pic.height = final_height;
                tmp_pic_scale.width = final_width;
                tmp_pic_scale.height = final_height;

                unsigned int count = 0;
                while ((entry = readdir(dir_p)))
                        if (bmp_extension(entry->d_name)) {
                                fprintf(write_cache, "%s\n", entry->d_name);

                                count++;
                                sprintf(tmp_path, "%s/%s", sign_path, entry->d_name);
                                readBMP(tmp_path, &tmp_pic.width, &tmp_pic.height, tmp_pic.arr);

                                SingleCrop(&tmp_pic, &tmp_pic);

                                Scale(&tmp_pic, &tmp_pic_scale);

                                sum_with(&tmp_avg_pic, &tmp_pic_scale);

                                // save the cropped scaled picture
                                sprintf(tmp_path, "%s/%c/O_%s", working_dir, sign, entry->d_name);
                                saveBMP(tmp_pic_scale.arr, tmp_pic_scale.width, tmp_pic_scale.height,
                                        tmp_path);
                                // make all elements of array zero
                                make_zero(&tmp_pic_scale);
                        }

                finalize_avg(&avg_pic, &tmp_avg_pic, count);

                // save the avg.bmp
                sprintf(tmp_path, "%s/%c/avg.bmp", working_dir, sign);
                saveBMP(avg_pic.arr, avg_pic.width, avg_pic.height, tmp_path);

                // threshold the avg_pic with poverty_line number.
                threshold_binary(&avg_pic, poverty_line(&avg_pic, 63));

                // save the avg_thr.bmp
                sprintf(tmp_path, "%s/%c/avg_thr.bmp", working_dir, sign);
                saveBMP(avg_pic.arr, avg_pic.width, avg_pic.height, tmp_path);

                // make all elements of array zero for next call
                make_zero(&avg_pic);
                make_zero_int(&tmp_avg_pic);

                // close the FILE , so buffer can be flushed
                fclose(write_cache);
        }

        closedir(dir_p);

        return 1;
}
