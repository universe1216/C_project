#ifndef _IMAGE_MANIPULATION_H_
#define _IMAGE_MANIPULATION_H_

#include "ppm.h"

void invertColor(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
		img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
		img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
	}
}

void grayscaleFilter(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double average = (img->data[i].red + img->data[i].green + img->data[i].blue) / 3.0;
		img->data[i].red = img->data[i].green = img->data[i].blue = average;
	}
}

void sepiaFilter(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double sepia_red = 0.393*img->data[i].red + 0.769*img->data[i].green + 0.189*img->data[i].blue;
		double sepia_green = 0.349*img->data[i].red + 0.686*img->data[i].green + 0.168*img->data[i].blue;
		double sepia_blue = 0.272*img->data[i].red + 0.534*img->data[i].green + 0.131*img->data[i].blue;
		img->data[i].red = truncateRGB(sepia_red);
		img->data[i].green = truncateRGB(sepia_green);
		img->data[i].blue = truncateRGB(sepia_blue);
	}
}

void rotateImage(PPMImage *img, short iteration) {
	unsigned short real_iteration = iteration>0 ? iteration%4 : (4 - (-iteration)%4) % 4, k;
	for(k=0;k<real_iteration;k++) {
		PPMImage *buffer = createPPMImage(img->width,img->height,BLACK);
		unsigned int i,j;
		for(i=0;i<img->height;i++)
			for(j=0;j<img->width;j++)
				buffer->data[img->height*(j+1) - i - 1] = img->data[img->width*i + j];
		copyPPMImage(img,buffer);
		free(buffer);
		unsigned int temp = img->height;
		img->height = img->width;
		img->width = temp;
	}
}

void resizeImage(PPMImage *img, unsigned int new_image_width, unsigned int new_image_height) {
	unsigned int width_ratio = (img->width<<16)/new_image_width + 1;
	unsigned int height_ratio = (img->height<<16)/new_image_height + 1, x2, y2, i, j;
	PPMImage *buffer = createPPMImage(new_image_width,new_image_height,BLACK);
	for(i=0;i<new_image_height;i++)
		for(j=0;j<new_image_width;j++) {
			x2 = (j*width_ratio)>>16;
			y2 = (i*height_ratio)>>16;
			buffer->data[j + i*new_image_width] = img->data[x2 + y2*img->width] ;
		}
	copyPPMImage(img,buffer);
}

void adjustSaturation(PPMImage *img, double saturation_percentage) {
	saturation_percentage/=100;
	if(saturation_percentage<-1)
		saturation_percentage=-1;
	else if(saturation_percentage>1)
		saturation_percentage=1;
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double *HSL=convertRGBtoHSL(img->data[i].red, img->data[i].green, img->data[i].blue);	
		if(saturation_percentage>=0) {
			double gray_factor = HSL[1];
			double var_interval = 1-HSL[1];
			HSL[1] += saturation_percentage*var_interval*gray_factor;
		}
		else {
			double var_interval = HSL[1]; 
			HSL[1] += saturation_percentage*var_interval;
		}
		double *RGB=convertHSLtoRGB(HSL[0],HSL[1],HSL[2]);
		img->data[i].red = RGB[0];
		img->data[i].green = RGB[1];
		img->data[i].blue = RGB[2];
	}
}

void adjustBlur(PPMImage *img, double sigma) {
	PPMImage *buffer=createPPMImage(img->width,img->height,BLACK);
	int rs = ceil(sigma*2.57), ix, iy, i, j;
	for(i=0;i<(signed int)img->height;i++)
		for(j=0;j<(signed int)img->width;j++) {
			double weighted_red=0, weighted_green=0, weighted_blue=0, weighted_sum=0;
			for(iy=i-rs;iy<i+rs+1;iy++)
				for(ix=j-rs;ix<j+rs+1;ix++) {
					int x = 0<ix ? (ix<(signed int)img->width-1 ? ix : (signed int)img->width-1) : (0<img->width-1 ? 0 : (signed int)img->width-1);
					int y = 0<iy ? (iy<(signed int)img->height-1 ? iy : (signed int)img->height-1) : (0<img->height-1 ? 0 : (signed int)img->height-1);
					double distance_squared = pow(ix-j,2) + pow(iy-i,2);
					double weight = exp(-distance_squared/(2*pow(sigma,2))) / (M_PI*2*pow(sigma,2));
					weighted_red += img->data[x + y*img->width].red * weight;
					weighted_green += img->data[x + y*img->width].green * weight;
					weighted_blue += img->data[x + y*img->width].blue * weight;
					weighted_sum += weight;
				}
			buffer->data[j + i*buffer->width].red = round(weighted_red/weighted_sum);
			buffer->data[j + i*buffer->width].green = round(weighted_green/weighted_sum);
			buffer->data[j + i*buffer->width].blue = round(weighted_blue/weighted_sum);
		}
	copyPPMImage(img,buffer);
}

void adjustContrast(PPMImage *img, double contrast_percentage) {
	contrast_percentage*=2.55;
	double factor = (259*(contrast_percentage + 255)) / (255*(259 - contrast_percentage));
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double contrast_red = factor*(img->data[i].red - 128) + 128;
		double contrast_green = factor*(img->data[i].green - 128) + 128;
		double contrast_blue = factor*(img->data[i].blue - 128) + 128;
		img->data[i].red = truncateRGB(contrast_red);
		img->data[i].green = truncateRGB(contrast_green);
		img->data[i].blue = truncateRGB(contrast_blue);
	}
}

void adjustBrightness(PPMImage *img, double brightness_percentage) {
	brightness_percentage*=2.55;
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		img->data[i].red = truncateRGB(img->data[i].red + brightness_percentage);
		img->data[i].green = truncateRGB(img->data[i].green + brightness_percentage);
		img->data[i].blue = truncateRGB(img->data[i].blue + brightness_percentage);
	}
}

#endif
