#include "project1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2)
{
    unsigned int r1, r2, g1, g2, b1, b2;
    r1 = c1.r;
    r2 = c2.r;
    g1 = c1.g;
    g2 = c2.g;
    b1 = c1.b;
    b2 = c2.b;
    return (r1==r2 && g1==g2 && b1==b2);
}

// check if image is all gray pixels
int img_grayscale(struct image *img)
{
    uint32_t w = img->w;
    uint32_t h = img->h;
    int len = w*h;
    struct rgb *ps = img->pixels;
    for (int i=0; i<len; i++) {
        struct rgb p = ps[i];
        unsigned int r = p.r;
        unsigned int g = p.g;
        unsigned int b = p.b;
        if (r!=g || g!=b)
            return 0;
    }
    return 1;
}

// return heap-allocated image struct given a P3 file and save max color depth
struct image *read_p3(int *max, int *len)
{
    char mn[3];
    uint32_t w, h;
    scanf("%2c %d %d %d",mn,&w,&h,max);
    mn[2] = '\0';
    if (strcmp(mn,"P3")!=0) {
        fprintf(stderr,"error: not given P3 file");
        exit(1);
    }
    struct image *img = (struct image*)malloc(sizeof(struct image));
    if (img==NULL) {
        fprintf(stderr,"error: malloc failed (read_p3)");
        exit(1);
    }
    img->w = w;
    img->h = h;
    *len = w*h;
    struct rgb *ps = (struct rgb*)malloc((*len)*sizeof(struct rgb));
    if (ps==NULL) {
        fprintf(stderr,"error: malloc failed (read_p3)");
        exit(1);
    }
    unsigned char r, g, b;
    for (int i=0; i<(*len); i++) {
        scanf("%hhu %hhu %hhu",&r,&g,&b);
        struct rgb p;
        p.r = r;
        p.g = g;
        p.b = b;
        ps[i] = p;
    } 
    img->pixels = ps; // note: ps is heap-allocated
    return img;
}
