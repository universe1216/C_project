#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// a pixel of the PPM image
typedef struct {
	unsigned char red, green, blue;		//color of the pixel is represented by the RGB (red/green/blue) scheme
} PPMPixel;

// a PPM image
typedef struct {
	unsigned int width, height;			//the width and height of an image in pixels
	PPMPixel *data;						//pixels that form the image
} PPMImage;

typedef struct {
	double x, y;
} Point;

#endif
