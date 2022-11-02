#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// write P6 file from P3 file (stdin) to stdout
void write_p6_from_p3()
{
    int max;
    int len;
    struct image *img = read_p3(&max,&len);
    int w = img->w;
    int h = img->h;
    fprintf(stdout,"P6\n%d %d\n%d\n",w,h,max);
    for (int i=0; i<len; i++) {
        struct rgb p = (img->pixels)[i];
        putchar(p.r);
        putchar(p.g);
        putchar(p.b);
    } 
    free(img->pixels);
    free(img);
}

int main(int argc, char *argv[])
{
    write_p6_from_p3();
    return 0;
}
