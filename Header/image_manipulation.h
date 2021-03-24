#ifndef _IMAGE_MANIPULATION_H_
#define _IMAGE_MANIPULATION_H_

#include "ppm.h"

// header file which contains basic image manipulation filters (invert color, sepia, grayscale) and techniques (rotate, resize, blur, adjust hue, etc.)

// aka negative filter, inverts the colors of the given image
void invertColor(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {			// since we are working with 8-bit RGB components, each pixel is converted from (r,g,b) to (255-r, 255-g, 255-b)
		img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
		img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
		img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
	}
}

// grayscale filter, converts the given image to only black and white
void meanGrayscaleFilter(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {			// each pixel is converted from (r,g,b) to (avg,avg,avg) where "avg = (r+g+b)/3" is the mean average of the RGB components
		double mean_average = (double)(img->data[i].red + img->data[i].green + img->data[i].blue) / 3;
		img->data[i].red = img->data[i].green = img->data[i].blue = mean_average;
	}
}

// another grayscale filter, which is adjusted for the human eye
void weightedGrayscaleFilter(PPMImage *img) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double weighted_average = 0.299*img->data[i].red + 0.587*img->data[i].green + 0.114*img->data[i].blue;
		img->data[i].red = img->data[i].green = img->data[i].blue = weighted_average;
	}
}

// sepia filter, converts the given image to a black-and-white image with a warm brownish tone
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

// pixelates the given image, meaning the pixels of the image is enlarged for anonymization
void pixelate(PPMImage *img, unsigned int pixel_size) {
	unsigned int i, j, k, l;
	for(i=0;i<img->width;i += pixel_size)
		for(j=0;j<img->height;j += pixel_size) {
			double red_average = 0, green_average = 0, blue_average = 0;
			unsigned int counter = 0;
			for(k=0;k<pixel_size;k++)
				for(l=0;l<pixel_size;l++)
					if(i + k < img->width && j + l < img->height) {
						red_average += img->data[i + k + (j + l)*img->width].red;
						green_average += img->data[i + k + (j + l)*img->width].green;
						blue_average += img->data[i + k + (j + l)*img->width].blue;
						counter++;
					}
			red_average /= counter;
			green_average /= counter;
			blue_average /= counter;
			for(k=0;k<pixel_size;k++)
				for(l=0;l<pixel_size;l++)
					if(i + k < img->width && j + l < img->height) {
						img->data[i + k + (j + l)*img->width].red = red_average;
						img->data[i + k + (j + l)*img->width].green = green_average;
						img->data[i + k + (j + l)*img->width].blue = blue_average;
					}
		}
}

// rotates the given image "iteration" times in the clockwise direction if the number is positive, counterclockwise if the number is negative
void rotateImage(PPMImage *img, short iteration) {
	unsigned short real_iteration = (iteration%4 + 4) % 4, k;
	for(k=0;k<real_iteration;k++) {
		PPMImage *buffer = createPPMImage(img->width,img->height,BLACK);
		unsigned int i, j;
		for(i=0;i<img->height;i++)
			for(j=0;j<img->width;j++)
				buffer->data[img->height*(j+1) - i - 1] = img->data[img->width*i + j];
		copyPPMImage(img,buffer);
		deletePPMImage(buffer);
		unsigned int temp = img->height;
		img->height = img->width;
		img->width = temp;
	}
}

// resizes the given image
void resizeImage(PPMImage *img, unsigned int new_image_width, unsigned int new_image_height) {
	unsigned int width_ratio = (img->width << 16) / new_image_width + 1;
	unsigned int height_ratio = (img->height << 16) / new_image_height + 1, x2, y2, i, j;
	PPMImage *buffer = createPPMImage(new_image_width,new_image_height,BLACK);
	for(i=0;i<new_image_height;i++)
		for(j=0;j<new_image_width;j++) {		// nearest neighbor algorithm
			x2 = (j*width_ratio) >> 16;
			y2 = (i*height_ratio) >> 16;
			buffer->data[j + i*new_image_width] = img->data[x2 + y2*img->width];
		}
	copyPPMImage(img,buffer);
	deletePPMImage(buffer);
}

// adjusts the saturation of the given image
void adjustSaturation(PPMImage *img, double saturation_percentage) {
	saturation_percentage /= 100;
	if(saturation_percentage < -1)
		saturation_percentage = -1;
	else if(saturation_percentage > 1)
		saturation_percentage = 1;
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double *HSL = convertRGBtoHSL(img->data[i].red, img->data[i].green, img->data[i].blue);	
		if(saturation_percentage>=0) {
			double gray_factor = HSL[1];
			double var_interval = 1 - HSL[1];
			HSL[1] += saturation_percentage * var_interval * gray_factor;
		}
		else {
			double var_interval = HSL[1]; 
			HSL[1] += saturation_percentage * var_interval;
		}
		double *RGB = convertHSLtoRGB(HSL[0], HSL[1], HSL[2]);
		img->data[i].red = RGB[0];
		img->data[i].green = RGB[1];
		img->data[i].blue = RGB[2];
	}
}

// blurs the image, where higher "sigma" values make the blur effect stronger
void adjustBlur(PPMImage *img, double sigma) {
	PPMImage *buffer = createPPMImage(img->width,img->height,BLACK);
	int rs = ceil(sigma*2.57), ix, iy, i, j;
	for(i=0;i<(signed int)img->height;i++)
		for(j=0;j<(signed int)img->width;j++) {
			double weighted_red = 0, weighted_green = 0, weighted_blue = 0, weighted_sum = 0;
			for(iy=i-rs;iy<i+rs+1;iy++)
				for(ix=j-rs;ix<j+rs+1;ix++) {
					int x = 0<ix ? (ix<(signed int)img->width-1 ? ix : (signed int)img->width-1) : (0<img->width-1 ? 0 : (signed int)img->width-1);
					int y = 0<iy ? (iy<(signed int)img->height-1 ? iy : (signed int)img->height-1) : (0<img->height-1 ? 0 : (signed int)img->height-1);
					double distance_squared = pow(ix-j,2) + pow(iy-i,2);
					double weight = exp(-distance_squared / (2 * pow(sigma,2))) / (M_PI * 2 * pow(sigma, 2));		// Gaussian blur
					weighted_red += img->data[x + y*img->width].red * weight;
					weighted_green += img->data[x + y*img->width].green * weight;
					weighted_blue += img->data[x + y*img->width].blue * weight;
					weighted_sum += weight;
				}
			buffer->data[j + i*buffer->width].red = round(weighted_red / weighted_sum);
			buffer->data[j + i*buffer->width].green = round(weighted_green / weighted_sum);
			buffer->data[j + i*buffer->width].blue = round(weighted_blue / weighted_sum);
		}
	copyPPMImage(img, buffer);
	deletePPMImage(buffer);
}

// adjusts the contrast of the given image
void adjustContrast(PPMImage *img, double contrast_percentage) {
	contrast_percentage *= 2.55;
	if(contrast_percentage < -255)
		contrast_percentage = -255;
	else if(contrast_percentage > 255)
		contrast_percentage = 255;
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

// adjusts the brightness of the given image, where positive percentages make the image brighter where as negative percentages make the image darker
void adjustBrightness(PPMImage *img, double brightness_percentage) {
	brightness_percentage *= 2.55;
	if(brightness_percentage < -255)
		brightness_percentage = -255;
	else if(brightness_percentage > 255)
		brightness_percentage = 255;
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		img->data[i].red = truncateRGB(img->data[i].red + brightness_percentage);
		img->data[i].green = truncateRGB(img->data[i].green + brightness_percentage);
		img->data[i].blue = truncateRGB(img->data[i].blue + brightness_percentage);
	}
}

// adjusts the hue of the given image
void adjustHue(PPMImage *img, int angle) {
	angle = (angle%360 + 360) % 360;
	const double cosine = cos((double)angle * M_PI / 180);
	const double sine = sin((double)angle * M_PI / 180);
	const double lum_r = 0.2126, lum_g = 0.7152, lum_b = 0.0722;
	const double hue_rotate_r = 0.143, hue_rotate_g = 0.140, hue_rotate_b = 0.283;
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		double RGB_matrix[3][3];
		RGB_matrix[0][0] = lum_r + (1-lum_r)*cosine - lum_r*sine;
		RGB_matrix[0][1] = lum_g - lum_g*cosine - lum_g*sine;
		RGB_matrix[0][2] = lum_b - lum_b*cosine + (1 - lum_b)*sine;
		RGB_matrix[1][0] = lum_r - lum_r*cosine + hue_rotate_r*sine;
		RGB_matrix[1][1] = lum_g + (1 - lum_g)*cosine + hue_rotate_g*sine;
		RGB_matrix[1][2] = lum_b - lum_b*cosine - hue_rotate_b*sine;
		RGB_matrix[2][0] = lum_r - lum_r*cosine - (1 - lum_r)*sine;
		RGB_matrix[2][1] = lum_g - lum_g*cosine + lum_g*sine;
		RGB_matrix[2][2] = lum_b + (1 - lum_b)*cosine + lum_b*sine;
		double hue_red = RGB_matrix[0][0] * img->data[i].red + RGB_matrix[0][1] * img->data[i].green + RGB_matrix[0][2] * img->data[i].blue;
		double hue_green = RGB_matrix[1][0] * img->data[i].red + RGB_matrix[1][1] * img->data[i].green + RGB_matrix[1][2] * img->data[i].blue;
		double hue_blue = RGB_matrix[2][0] * img->data[i].red + RGB_matrix[2][1] * img->data[i].green + RGB_matrix[2][2] * img->data[i].blue;
		img->data[i].red = truncateRGB(hue_red);
		img->data[i].green = truncateRGB(hue_green);
		img->data[i].blue = truncateRGB(hue_blue);
	}
}

// applies gamma correction on the given image
void gammaCorrection(PPMImage *img, double gamma) {
	unsigned int i;
	for(i=0;i<img->width*img->height;i++) {
		img->data[i].red = RGB_COMPONENT_COLOR * pow((double)img->data[i].red/RGB_COMPONENT_COLOR, gamma);
		img->data[i].green = RGB_COMPONENT_COLOR * pow((double)img->data[i].green/RGB_COMPONENT_COLOR, gamma);
		img->data[i].blue = RGB_COMPONENT_COLOR * pow((double)img->data[i].blue/RGB_COMPONENT_COLOR, gamma);
	}
}

#endif
