#include <stdio.h>
#include "err.h"

char input_int(int *data, int start, int end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%9d%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data < start) || (*data > end)){
        if(flag_scanf == EOF){
            return -1;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%9d%c", data, &end_of_scanf);
    }
    return 0;
}

char input_uint(unsigned *data, unsigned start, unsigned end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%9u%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data < start) || (*data > end)){
        if(flag_scanf == EOF){
            return -1;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%9u%c", data, &end_of_scanf);
    }
    return 0;
}

char input_long(long *data, long start, long end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%9ld%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data < start) || (*data > end)){
        if(flag_scanf == EOF){
            return -1;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%9ld%c", data, &end_of_scanf);
    }
    return 0;
}

char input_double(double *data, double start, double end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%lf%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data <= start) || (*data >= end)){
        if(flag_scanf == EOF){
            return 1;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%lf%c", data, &end_of_scanf);
    }
    return 0;
}

char input_float(float *data, float start, float end){
    char end_of_scanf = ' ';
    int flag_scanf = scanf("%f%c", data, &end_of_scanf);

    while((flag_scanf != 2) || (end_of_scanf != '\n') || (*data <= start) || (*data >= end)){
        if(flag_scanf == EOF){
            return 1;
        }
        if((flag_scanf != 2) || (end_of_scanf != '\n')){
            scanf("%*[^\n]");
        }
        printf("Повторите ввод: ");
        flag_scanf = scanf("%f%c", data, &end_of_scanf);
    }
    return 0;
}
