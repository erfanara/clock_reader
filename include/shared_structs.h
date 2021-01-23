// Include guard
#ifndef __SHARED_STRUCTS__H
#define __SHARED_STRUCTS__H


/* Every picture has a 3D array and a width and a height
 */
typedef struct picture {
        unsigned char arr[2000][2000][3];
        int width, height;
} picture;

typedef struct int_picture {
        unsigned int arr[2000][2000][3];
        int width, height;
} int_picture;

#endif
