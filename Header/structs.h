#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct {
	unsigned char red,green,blue;
} PPMPixel;

typedef struct {
	unsigned int width,height;
	PPMPixel *data;
} PPMImage;

typedef struct {
	double x,y;
} Point;

#endif
