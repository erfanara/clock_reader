// Mohammad Erfan Arasteh 9912762427

#include "./include/AutoCrop.h"
#include "./include/AvgPic.h"
#include "./include/Recognition.h"
#include "./include/Scale.h"
#include "./include/dir_utils.h"
#include <gtk/gtk.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
        char path[256];
        char a[100];
        printf("DataSet path:\n");
        scanf("%100s", a);
        for (int i = 0; i <= 9; i++) {
                sprintf(path, "%s/%d", a, i);
                AvgPic(path, 300, 500, ".DATA_SET_C");
        }

        printf("input picture path:\n");
        scanf("%256s", a);
        AutoCrop(a, "./.DATA_SET_C");

        printf("\n");
        printf("%d", Recognition_pix("./.DATA_SET_C/0_0.bmp", "./.DATA_SET_C"));
        printf("%d", Recognition_pix("./.DATA_SET_C/0_1.bmp", "./.DATA_SET_C"));
        printf(":");
        printf("%d", Recognition_pix("./.DATA_SET_C/0_3.bmp", "./.DATA_SET_C"));
        printf("%d", Recognition_pix("./.DATA_SET_C/0_4.bmp", "./.DATA_SET_C"));
        printf(":");
        printf("%d", Recognition_pix("./.DATA_SET_C/0_6.bmp", "./.DATA_SET_C"));
        printf("%d", Recognition_pix("./.DATA_SET_C/0_7.bmp", "./.DATA_SET_C"));


        return 0;
}
