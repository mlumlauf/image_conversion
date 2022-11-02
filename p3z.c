#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// return heap-allocated array of run_lengths and compute number of runs
uint32_t *run_lengths(struct rgb *pixels, int len, uint32_t *nruns)
{
    uint32_t *lengths = (uint32_t*)malloc(len*sizeof(uint32_t));
    if (lengths==NULL) {
        fprintf(stderr,"error: malloc failed (run_lengths)");
        exit(1);
    } 
    uint32_t runs = 0;
    int j = 0;
    for (int i=0; i<len; i++) {
        int length = 1; 
        while (i+1<len && rgb_eq(pixels[i],pixels[i+1])) {
            length++;
            i++;
        }  
        lengths[j++] = length; 
        runs++; 
    }
    *nruns = runs;
    return lengths;
}

// return heap-allocated array of grayscale colors (use if image is grayscale)
unsigned char *colors_gray(struct rgb *pixels, uint32_t *ls, uint32_t nruns)
{
    unsigned char *colors = (unsigned char*)malloc(nruns*sizeof(unsigned char)); 
    if (colors==NULL) {
        fprintf(stderr,"error: malloc failed (colors_gray)");
        exit(1);
    }
    int index = 0;
    for (int i=0; i<nruns; i++) {
        colors[i] = (pixels[index]).r;
        index += ls[i]; 
    }
    return colors;
}

// return heap-allocated array of rgb colors (use if image is not grayscale)
struct rgb *colors_rgb(struct rgb *pixels, uint32_t *ls, uint32_t nruns)
{
    struct rgb *colors = (struct rgb*)malloc(nruns*sizeof(struct rgb)); 
    if (colors==NULL) {
        fprintf(stderr,"error: malloc failed (colors_rbg)");
        exit(1);
    }
    int index = 0;
    for (int i=0; i<nruns; i++) {
        colors[i] = pixels[index];
        index += ls[i]; 
    }
    return colors;
}

// return date as integer and update current time 
uint32_t get_time_info(uint16_t *hr_min)
{
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    int hr = tm_info->tm_hour;
    int min = tm_info->tm_min;
    *hr_min = (hr*100) + min;
    int d = tm_info->tm_mday;
    int m = (tm_info->tm_mon)+1;
    int y = 1900 + (tm_info->tm_year);
    uint32_t date = (y*10000) + (m*100) + d;
    return date;
}

// write PZ image to stdout given P3 image
void write_pz_from_p3(char *descrip)
{
    int max; 
    int len;
    struct image *img = read_p3(&max,&len);
    struct rgb *ps = img->pixels;
    uint32_t nruns;
    uint32_t *ls = run_lengths(ps,len,&nruns);
    putchar('P');
    putchar('Z');
    putchar('.'); // next two bytes reserved for future 
    putchar('.'); 
    uint16_t hr_min;
    uint32_t date = get_time_info(&hr_min);
    putchar((hr_min>>8)&0xFF);
    putchar(hr_min&0xFF);
    putchar((date>>24)&0xFF);
    putchar((date>>16)&0xFF);
    putchar((date>>8)&0xFF);
    putchar(date&0xFF);
    uint16_t width = img->w;
    uint16_t height = img->h;
    putchar((width>>8)&0xFF);
    putchar(width&0xFF);
    putchar((height>>8)&0xFF);
    putchar(height&0xFF);
    putchar('.'); // next four bytes reserved for future expansion
    putchar('.');
    putchar('.');
    putchar('.');
    unsigned char byte_18 = '\0';
    int gray = img_grayscale(img);
    if (gray)
        byte_18 = 0x03;
    else if (!gray) 
        byte_18 = 0x01; 
    putchar(byte_18);
    putchar('.'); // next three bytes reserved for future expansion
    putchar('.');
    putchar('.');
    int i = 0;
    while (descrip[i]!='\0') 
        putchar(descrip[i++]);
    putchar('\0');
    putchar((nruns>>24)&0xFF);
    putchar((nruns>>16)&0xFF);
    putchar((nruns>>8)&0x0FF);
    putchar(nruns&0xFF);
    for (int i=0; i<nruns; i++) {
        uint32_t l = ls[i];
        putchar((l>>24)&0xFF);
        putchar((l>>16)&0xFF);
        putchar((l>>8)&0xFF);
        putchar(l&0xFF);
    }
    if (gray) { 
        unsigned char *cs = colors_gray(ps,ls,nruns);
        free(ls);
        free(ps);
        for (int i=0; i<nruns; i++) {
            unsigned char clr = cs[i];
            putchar(clr);
        }
        free(cs);
    }
    else if (!gray) {
        struct rgb *cs = colors_rgb(ps,ls,nruns); 
        free(ls);
        free(ps);
        for (int i=0; i<nruns; i++) {
            struct rgb clr = cs[i];
            putchar(clr.r);
            putchar(clr.g);
            putchar(clr.b);
        }
        free(cs);
    } 
    free(img);
}

int main(int argc, char *argv[])
// assumes only arguments are program name and optional description
// note that files are not included in command line arguments
{
    if (argc==1) {
        char *empty_descrip = "";
        write_pz_from_p3(empty_descrip);
    } 
    else { 
        char *descrip = argv[1];
        write_pz_from_p3(descrip);
    }
    return 0;
}
