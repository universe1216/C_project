#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "functions.h"
#include "ppm.h"

static PPMImage* printRGBstrips(unsigned int width, unsigned int height, unsigned int iteration) {
	PPMImage *img = createPPMImage(width,3*height*iteration,BLACK);
	unsigned int i,j,k,counter=0;
	for(k=0;k<iteration;k++) {
		for(i=0;i<height;i++)
			for(j=0;j<width;j++) {
				img->data[counter].red = j%(RGB_COMPONENT_COLOR+1);
				img->data[counter].green = 0;
				img->data[counter].blue = 0;
				counter++;
			}
		for(i=0;i<height;i++)
			for(j=0;j<width;j++) {
				img->data[counter].red = 0;
				img->data[counter].green = j%(RGB_COMPONENT_COLOR+1);
				img->data[counter].blue = 0;
				counter++;
			}
		for(i=0;i<height;i++)
			for(j=0;j<width;j++) {
				img->data[counter].red = 0;
				img->data[counter].green = 0;
				img->data[counter].blue = j%(RGB_COMPONENT_COLOR+1);
				counter++;
			}
	}
	return img;
}

static PPMImage* printRainbow(unsigned int width, unsigned int height, unsigned int iteration) {
	PPMImage *img = createPPMImage(width,7*height*iteration,BLACK);
	unsigned int i,j,k,counter=0;
	for(k=0;k<iteration;k++) {
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = RED;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = ORANGE;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = YELLOW;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = GREEN;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = BLUE;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = INDIGO;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = DARK_VIOLET;
	}
	return img;
}

void drawCircle(int xc, int yc, int x, int y, PPMImage *img, PPMPixel boundary_color) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	img->data[center_index + xc + x - (yc + y)*img->width - 1] = boundary_color;
	img->data[center_index + xc - x - (yc + y)*img->width - 1] = boundary_color;
	img->data[center_index + xc + x - (yc - y)*img->width - 1] = boundary_color;
	img->data[center_index + xc - x - (yc - y)*img->width - 1] = boundary_color;
	img->data[center_index + xc + y - (yc + x)*img->width - 1] = boundary_color;
	img->data[center_index + xc - y - (yc + x)*img->width - 1] = boundary_color;
	img->data[center_index + xc + y - (yc - x)*img->width - 1] = boundary_color;
	img->data[center_index + xc - y - (yc - x)*img->width - 1] = boundary_color;
}

void printCircles(int xc, int yc, unsigned int r, unsigned int gap, unsigned int frequency, PPMImage *img, PPMPixel boundary_color) {
	while(frequency--) {
		int x=0, y=r;
		int d=3-2*r;
		drawCircle(xc,yc,x,y,img,boundary_color);
		while(y>=x) {
			x++;
			if(d>0) {
				y--;
				d+=4*(x-y)+10;
			}
			else
				d+=4*x+6;
			drawCircle(xc,yc,x,y,img,boundary_color);
		}
		r-=gap;
	}
}

static PPMImage* printRoughHeart(unsigned int size) {
	PPMImage *img = size%4==1 || size%4==2 ? createPPMImage(2*size-1, floor(5*size/4.0)+1, BLACK) : createPPMImage(2*size, floor(5*size/4.0)+2, BLACK);
	unsigned int i,j,counter=0;
 	for(i=size/2;i<=size;i+=2) {
		for(j=1;j<size-i;j+=2)
			img->data[counter++] = BLACK;
		for(j=1;j<=i;j++)
			img->data[counter++] = RED;
		for(j=1;j<=size-i;j++)
			img->data[counter++] = BLACK;
		for(j=1;j<=i;j++)
			img->data[counter++] = RED;
		for(j=1;j<size-i;j+=2)
			img->data[counter++] = BLACK;
	}
	for(i=size;i>=1;i--) {
		for(j=i;j<size;j++)
			img->data[counter++] = BLACK;
		for(j=1;j<=2*i-1;j++)
			img->data[counter++] = RED;
		for(j = size%4==1 || size%4==2 ? i : i-1;j<size;j++)
			img->data[counter++] = BLACK;
	}
	return img;
}

void drawLine(int x1, int y1, int x2, int y2, PPMImage *img, PPMPixel line_color) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	int dx=x2-x1, dy=y2-y1;
	unsigned int steps = max(abs(dx),abs(dy));
	float Xinc = dx/(float)steps, Yinc = dy/(float)steps, x=x1, y=y1;
	unsigned int i;
	for(i=0; i<=steps; i++, x+=Xinc, y+=Yinc)
		img->data[center_index + (int)x - img->width*(int)y - 1] = line_color;
}

void fillRegion(int x, int y, PPMPixel fill_color, PPMPixel region_color, PPMImage *img) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	if(arePixelsEqual(img->data[center_index + x - img->width*y - 1], region_color) && !arePixelsEqual(img->data[center_index + x - img->width*y - 1], fill_color)) {
		img->data[center_index + x - img->width*y - 1] = fill_color;
		fillRegion(x-1,y,fill_color,region_color,img);
		fillRegion(x+1,y,fill_color,region_color,img);
		fillRegion(x,y-1,fill_color,region_color,img);
		fillRegion(x,y+1,fill_color,region_color,img);
	}
}

#endif
