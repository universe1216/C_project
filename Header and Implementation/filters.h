#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "functions.h"
#include "ppm.h"

void invertColorPPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {
			img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
			img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
			img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
		}
}

void grayscalePPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {
			double average = (img->data[i].red + img->data[i].green + img->data[i].blue) / 3.0;
			img->data[i].red = img->data[i].green = img->data[i].blue = average;
		}
}

void sepiaPPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {
			double sepia_red = 0.393*img->data[i].red + 0.769*img->data[i].green + 0.189*img->data[i].blue;
			double sepia_green = 0.349*img->data[i].red + 0.686*img->data[i].green + 0.168*img->data[i].blue;
			double sepia_blue = 0.272*img->data[i].red + 0.534*img->data[i].green + 0.131*img->data[i].blue;
			img->data[i].red = sepia_red>255 ? 255 : sepia_red;
			img->data[i].green = sepia_green>255 ? 255 : sepia_green;
			img->data[i].blue = sepia_blue>255 ? 255 : sepia_blue;
		}
}

void rotate(PPMImage *img, short iteration) {
	unsigned short real_iteration = iteration>0 ? iteration%4 : (4 - (-iteration)%4) % 4, k;
	if(img)
		for(k=0;k<real_iteration;k++) {
			PPMImage *buffer = createPPMImage(img->width,img->height,BLACK);
			unsigned int i,j;
			for(i=0;i<img->height;i++)
				for(j=0;j<img->width;j++)
					buffer->data[img->height*(j+1) - i - 1] = img->data[img->width*i + j];
			memcpy(img->data, buffer->data, img->width * img->height * sizeof(PPMPixel));
			free(buffer);
			unsigned int temp = img->height;
			img->height = img->width;
			img->width = temp;
		}
}

#endif
