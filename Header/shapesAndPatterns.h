#ifndef _SHAPES_AND_PATTERNS_H_
#define _SHAPES_AND_PATTERNS_H_

#include "ppm.h"

// header file that contains circle, line drawing and color filling techniques

// a utility function of "printMultipleCircles", prints 8 points of the circle centered at (xc, yc)
void drawCircle(int xc, int yc, int x, int y, PPMImage *img, PPMPixel color) {
	putPixel(xc+x, yc+y, img, color);
	putPixel(xc-x, yc+y, img, color);
	putPixel(xc+x, yc-y, img, color);
	putPixel(xc-x, yc-y, img, color);
	putPixel(xc+y, yc+x, img, color);
	putPixel(xc-y, yc+x, img, color);
	putPixel(xc+y, yc-x, img, color);
	putPixel(xc-y, yc-x, img, color);
}

// draws multiple circles centered at (xc, yc) on the given image where:
// radius: the radius of the outermost circle
// gap: gap between circles
// frequency: number of circles
void printMultipleCircles(int xc, int yc, unsigned int radius, unsigned int gap, unsigned int frequency, PPMImage *img, PPMPixel color) {
	while(frequency--) {
		int x=0, y=radius, d=3-2*radius;
		drawCircle(xc, yc, x, y, img, color);
		while(y>=x) {			//Bresenham's circle drawing algorithm
			x++;
			if(d>0) {
				y--;
				d += 4*(x-y)+10;
			}
			else
				d += 4*x+6;
			drawCircle(xc, yc, x, y, img, color);
		}
		radius -= gap;
	}
}

// draws a line from (x1,y1) to (x2,y2) on the given image
void drawLine(int x1, int y1, int x2, int y2, PPMImage *img, PPMPixel color) {
	int dx=x2-x1, dy=y2-y1;
	unsigned int steps = abs(dx)>abs(dy) ? abs(dx) : abs(dy), i;
	double Xinc = dx/(double)steps, Yinc = dy/(double)steps, x=x1, y=y1;
	for(i=0; i<=steps; i++, x+=Xinc, y+=Yinc)					//DDA (Digital Differential Analyzer) line drawing algorithm
		putPixel((int)x, (int)y, img, color);
}

// fills a region on the given image
void fillRegion(int x, int y, PPMPixel fill_color, PPMPixel region_color, PPMImage *img) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	if(arePixelsEqual(img->data[center_index + x - img->width*y - 1], region_color) && !arePixelsEqual(img->data[center_index + x - img->width*y - 1], fill_color)) {
		putPixel(x, y, img, fill_color);
		fillRegion(x-1, y, fill_color, region_color, img);		//recursive flood fill with 4 connected pixels algorithm
		fillRegion(x+1, y, fill_color, region_color, img);
		fillRegion(x, y-1, fill_color, region_color, img);
		fillRegion(x, y+1, fill_color, region_color, img);
	}
}

#endif
