#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// skip n number of bytes
void skip_bytes(int n)
{
    unsigned char byte;
    for (int i=0; i<n; i++) 
        byte = getchar();
}

// convert two byte big-edian integer into 32-bit integer
uint32_t convert_2big(unsigned char n1, unsigned char n2)
{
    return (0x00<<24 | 0x00<<16 | n1<<8 | n2);
}

// convert four byte big-endian integer into 32-bit integer
uint32_t convert_4big(unsigned char n1, unsigned char n2, 
                      unsigned char n3, unsigned char n4)
{
    return (n1<<24 | n2<<16 | n3<<8 | n4);
}

// return a heap-allocated array of pixels 
// from arrays of run lengths and corresponding colors
struct rgb *pixel_arr(uint32_t *ls, struct rgb *cs, uint32_t nruns, int len)
{
    struct rgb *pixels = (struct rgb*)malloc(len*sizeof(struct rgb));
    if (pixels==NULL) {
        fprintf(stderr,"error: malloc failed (pixel_arr)");
        exit(1);
    }
    int pos = 0;
    for (int i=0; i<nruns; i++) {
        uint32_t run_length = ls[i];
        struct rgb color = cs[i];
        for (int j=0; j<run_length; j++) 
            pixels[j+pos] = color; 
        pos += run_length;
    }         
    return pixels;
}

// return heap-allocated array of run lengths from the PZ file
uint32_t *lengths_arr(uint32_t nruns)
{
    uint32_t *lengths = (uint32_t*)malloc(nruns*sizeof(uint32_t));
    if (lengths==NULL) {
        fprintf(stderr,"error: malloc failed (lengths_arr)");
        exit(1);
    }
    for (int i=0; i<nruns; i++) {
        unsigned char rl1, rl2, rl3, rl4;
        rl1 = getchar();
        rl2 = getchar();
        rl3 = getchar();
        rl4 = getchar();
        lengths[i] = convert_4big(rl1,rl2,rl3,rl4);
    }
    return lengths;
}

// return a heap-allocated array of colors from the PZ file
struct rgb *colors_arr(uint32_t nruns, int gray)
{
    struct rgb *colors = (struct rgb*)malloc(nruns*sizeof(struct rgb));
    if (colors==NULL) {
        fprintf(stderr,"error: malloc failed (colors_arr)");
        exit(1);
    } 
    if (gray) 
        for (int i=0; i<nruns; i++) {
            unsigned char color;
            color = getchar();
            struct rgb clr;
            clr.r = color;
            clr.g = color;
            clr.b = color; 
            colors[i] = clr;
        }
    else
        for (int i=0; i<nruns; i++) {
            unsigned char r, g, b;
            r = getchar();
            g = getchar();
            b = getchar();
            struct rgb clr;
            clr.r = r;
            clr.g = g;
            clr.b = b;
            colors[i] = clr;
        }      
    return colors;
}

// return heap-allocated image struct given a PZ file
struct image *read_pz()
{
    unsigned char mn1, mn2;
    mn1 = getchar();
    mn2 = getchar();
    if (mn1!='P' || mn2!='Z') {
        fprintf(stderr,"error: not given PZ file");
        exit(1);
    }
    skip_bytes(2); // reserved for future expansion 
    skip_bytes(2); // skip current time
    skip_bytes(4); // skip current date
    unsigned char w1, w2, h1, h2;
    w1 = getchar();
    w2 = getchar();
    h1 = getchar();
    h2 = getchar();
    struct image *img = (struct image*)malloc(sizeof(struct image));
    if (img==NULL) {
        fprintf(stderr,"error: malloc failed (read_pz)");
        exit(1);
    }
    uint32_t width = convert_2big(w1,w2);
    uint32_t height = convert_2big(h1,h2);
    img->w = width;
    img->h = height;
    skip_bytes(4); // reserved for future expansion
    unsigned char bitmap;
    bitmap = getchar();
    // int run_encoded = bitmap&0x01; note: always 1 for this project (unused)
    int gray = bitmap&0x02;
    skip_bytes(3); // reserved for future expansion
    int b = getchar();
    while (b!='\0') // skip description bytes
        b = getchar();
    unsigned char r1, r2, r3, r4;
    r1 = getchar();
    r2 = getchar();
    r3 = getchar();
    r4 = getchar();
    uint32_t nruns = convert_4big(r1,r2,r3,r4);
    uint32_t *lengths = lengths_arr(nruns);
    struct rgb *colors = colors_arr(nruns,gray); 
    int len = width*height;
    struct rgb *ps = pixel_arr(lengths,colors,nruns,len);
    free(lengths);
    free(colors);
    img->pixels = ps; // note that pixels array is heap-allocated
    return img; 
}

// write P3 file from PZ file (stdin) to stdout
void write_p3_from_pz()
{
    int max = 255;
    struct image *img = read_pz();
    int w = img->w;
    int h = img->h;
    fprintf(stdout,"P3\n%d %d\n%d\n",w,h,max);
    int len = w*h;
    for (int i=0; i<len; i++) {
        struct rgb p = (img->pixels)[i];
        fprintf(stdout,"%hhu %hhu %hhu ",p.r,p.g,p.b);
    } 
    free(img->pixels);
    free(img);
}

int main(int argc, char *argv[])
{
    write_p3_from_pz();
    return 0;
}
