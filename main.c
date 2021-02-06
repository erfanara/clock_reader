// Mohammad Erfan Arasteh 9912762427

// Include dependencies
#include "./include/AutoCrop.h"
#include "./include/AvgPic.h"
#include "./include/Recognition.h"
#include "./include/Scale.h"
#include "./include/dir_utils.h"
#include <stdio.h>


/*********************************************************
 *                  ** GLOBAL VARS **                    *
 *********************************************************/
#if defined(__unix__)
const char *CLEAR = "clear";
#elif defined(_WIN32)
const char *CLEAR = "cls";
#endif

const char *LOGO = "   ________           __      ____                 __         \n"
                   "  / ____/ /___  _____/ /__   / __ \\___  ____ _____/ /__  _____\n"
                   " / /   / / __ \\/ ___/ //_/  / /_/ / _ \\/ __ `/ __  / _ \\/ ___/\n"
                   "/ /___/ / /_/ / /__/ ,<    / _, _/  __/ /_/ / /_/ /  __/ /    \n"
                   "\\____/_/\\____/\\___/_/|_|  /_/ |_|\\___/\\__,_/\\__,_/\\___/_/   V4.0\n";

const char *author = "M.Erfan Arasteh";

const char *help_menu = "\nPlease choose one number:\n"
                        "0) Specify Dataset & Recognize clock from bmp file\n"
                        "1) Specify DataSet & Create cache from DataSets\n"
                        "2) Just recognize clock from bmp file without creating cache\n"
                        "3) AutoCrop\n"
                        "4) Just remove White Rows/columns from input bmp file\n"
                        "5) Scale\n"
                        "6) Threshold\n\n";

const char *CACHE_PATH = ".DATA_SET_C";
char DATASET_PATH[125] = "DataSet";

int preferred_width = 300, preferred_height = 500;

picture input_pic, output_pic;

/*********************************************************
 *              ** FUNCTION DEFINITIONS **               *
 *********************************************************/
void Dataset_and_Recognize() {
        printf("0) Specify Dataset & Recognize clock from bmp file\n\n");

        char tmp_path[256];
        char input[100];
        printf("DataSet path:  ");
        scanf("%s", input);

        for (int i = 0; i <= 9; i++) {
                sprintf(tmp_path, "%s/%d", input, i);
                if (!AvgPic(tmp_path, preferred_width, preferred_height, CACHE_PATH))
                        return;
        }

        printf("input picture path:  ");
        scanf("%s", input);

        AutoCrop(input, (char *)CACHE_PATH);

        printf("\n");
        for (int i = 0; i <= 7; i++) {
                if (i == 2 || i == 5)
                        printf(":");
                else {
                        sprintf(tmp_path, "./%s/0_%d.bmp", CACHE_PATH, i);
                        printf("%d",
                               Recognition_pix(tmp_path, CACHE_PATH, preferred_width, preferred_height));
                }
        }
        printf("\n\n");
}

void specify_DataSet_Cache() {
        printf("1) Specify DataSet & Create cache from DataSets\n\n");

        char tmp_path[256];
        char input[100];
        printf("DataSet path:  ");
        scanf("%s", input);

        printf("Your preferred width:  ");
        scanf("%d", &preferred_width);
        printf("Your preferred height:  ");
        scanf("%d", &preferred_height);

        for(int i=0;i<=9;i++){
                sprintf(tmp_path,"%s/.cache/%d.cache",CACHE_PATH,i);
                remove(tmp_path);
        }

        for (int i = 0; i <= 9; i++) {
                sprintf(tmp_path, "%s/%d", input, i);
                if (!AvgPic(tmp_path, preferred_width, preferred_height, CACHE_PATH))
                        return;
        }
}

void just_recognize() {
        printf("2) Just recognize clock from bmp file without creating cache\n\n");

        printf("input picture path:  ");
        char tmp_path[256];
        char input[100];
        scanf("%s", input);

        AutoCrop(input, (char *)CACHE_PATH);

        printf("\n");
        for (int i = 0; i <= 7; i++) {
                if (i == 2 || i == 5)
                        printf(":");
                else {
                        sprintf(tmp_path, "./%s/0_%d.bmp", CACHE_PATH, i);
                        printf("%d",
                               Recognition_pix(tmp_path, CACHE_PATH, preferred_width, preferred_height));
                }
        }
        printf("\n\n");
}

void just_AutoCrop() {
        printf("3) AutoCrop\n\n");

        char output_dir[100];
        char input[100];
        printf("input picture path:  ");
        scanf("%s", input);
        printf("output directory path:  ");
        scanf("%s", output_dir);

        AutoCrop(input, output_dir);
}

void just_Remove_WhiteLines() {
        printf("4) Just remove White Rows/columns from input bmp file\n\n");

        char output[100];
        char input[100];
        printf("input picture path:  ");
        scanf("%s", input);
        printf("output picture path:  ");
        scanf("%s", output);

        readBMP(input, &input_pic.width, &input_pic.height, input_pic.arr);
        SingleCrop(&input_pic, &output_pic);
        saveBMP(output_pic.arr, output_pic.width, output_pic.height, output);

        make_zero(&output_pic);
}

void just_scale() {
        printf("5) Scale\n\n");

        char output[100];
        char input[100];
        printf("input picture path:  ");
        scanf("%s", input);
        printf("output picture path:  ");
        scanf("%s", output);

        printf("Your preferred width:  ");
        scanf("%d", &output_pic.width);
        printf("Your preferred height:  ");
        scanf("%d", &output_pic.height);

        readBMP(input, &input_pic.width, &input_pic.height, input_pic.arr);
        Scale(&input_pic, &output_pic);
        saveBMP(output_pic.arr, output_pic.width, output_pic.height, output);

        make_zero(&output_pic);
}

void just_threshold() {
        printf("6) Threshold\n\n");

        char output[100];
        char input[100];
        printf("input picture path:  ");
        scanf("%s", input);
        printf("output picture path:  ");
        scanf("%s", output);

        int thr_num;
        printf("Your preferred Threshold number:  ");
        scanf("%d", &thr_num);

        readBMP(input, &input_pic.width, &input_pic.height, input_pic.arr);
        threshold_binary(&input_pic, thr_num);
        saveBMP(input_pic.arr, input_pic.width, input_pic.height, output);
}

int main(int argc, char *argv[]) {
        printf("%s", LOGO);
        printf("\n\t%s\n\n\n", author);

        printf(" Write help for help menu\n\n");

        while (1) {
                printf("CR>");
                char user_input[10];
                scanf("%s", user_input);
                system(CLEAR);
                if (strlen(user_input) == 1) {
                        switch (user_input[0]) {
                        case '0':
                                Dataset_and_Recognize();
                                break;
                        case '1':
                                specify_DataSet_Cache();
                                break;
                        case '2':
                                just_recognize();
                                break;
                        case '3':
                                just_AutoCrop();
                                break;
                        case '4':
                                just_Remove_WhiteLines();
                                break;
                        case '5':
                                just_scale();
                                break;
                        case '6':
                                just_threshold();
                                break;
                        default:
                                printf("%s", help_menu);
                                break;
                        }
                } else
                        printf("%s", help_menu);
        }

}
