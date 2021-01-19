/*              ** STRUCTURES **                */

/* Every picture has a 3D array and a width and a height
 * and since a picture has huge array we'll declare
 * three picture globally:
 *      oldPic            >> We use this for saving input picture
 *      newPic_row        >> We use this for scaling the oldPic in rows
 *      newPic_row_column >> We use this for scaling thw newPic_row in columns
 */
typedef struct picture {
        unsigned char arr[2000][2000][3];
        int width, height;
} picture;

