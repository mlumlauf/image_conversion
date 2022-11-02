#ifndef _PROJECT1_H_
#define _PROJECT1_H_

#include <stdint.h>

struct rgb {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

struct image {
  uint32_t w;
  uint32_t h;
  struct rgb *pixels; // array of length w*h
};

// an image's pixels are stored in "row order",
// with the top row first from left to right,
// then the next row underneath, etc.

/* implement these two utilities in project1-utils.c */

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2);

// check if image is all gray pixels
int img_grayscale(struct image *img);

// return heap-allocated image struct given a P3 file and save max color depth
struct image *read_p3(int *max, int *len);

#endif
