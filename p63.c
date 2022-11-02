#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// return heap-allocated image struct given a P6 file and save max color depth
struct image *read_p6(int *max)
{
    char mn[3];
    uint32_t w, h;
    scanf("%2c %d %d %d ",mn,&w,&h,max);
    mn[2] = '\0';
    if (strcmp(mn,"P6")!=0) {
        fprintf(stderr,"error: not given P6 file");
        exit(1);
    }
    struct image *img = (struct image*)malloc(sizeof(struct image));
    if (img==NULL) {
        fprintf(stderr,"error: malloc failed (read_p6)");
        exit(1);
    }
    img->w = w;
    img->h = h;
    int len = w*h;
    struct rgb *ps = (struct rgb*)malloc(len*sizeof(struct rgb));
    unsigned char r, g, b;
    for (int i=0; i<len; i++) {
        r = getchar();
        g = getchar();
        b = getchar();
        struct rgb p;
        p.r = r;
        p.g = g;
        p.b = b;
        ps[i] = p;
    } 
    img->pixels = ps; // note: ps is heap-allocated
    return img;
}

// write P3 file from P6 file (stdin) to stdout
void write_p3_from_p6()
{
    int max;
    struct image *img = read_p6(&max);
    int w = img->w;
    int h = img->h;
    fprintf(stdout,"P3\n%d %d\n%d\n",w,h,max);
    int len = w*h;
    for (int i=0; i<len; i++) {
        struct rgb p = (img->pixels)[i];
        fprintf(stdout,"%d %d %d ",p.r,p.g,p.b);
    } 
    free(img->pixels);
    free(img);
}

int main(int argc, char *argv[])
{
    write_p3_from_p6();
    return 0;
}
