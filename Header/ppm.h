#ifndef _PPM_H_
#define _PPM_H_

#include "declarations.h"

// header file which contains basic operations for PPM files such as read, write, copy, etc. and also contains color conversion methods
// the code for readPPM and writePPM is taken from: https://stackoverflow.com/questions/2693631/read-ppm-file-and-store-it-in-an-array-coded-with-c

// reads the PPM file located in the given directory "filename" and returns a pointer to the stored data
PPMImage* readPPM(const char *filename) {
	char buff[16];
	PPMImage *img;
	FILE *fp;
	int rgb_comp_color;
	fp = fopen(filename, "rb");						// open PPM file for reading
	if(!fp) {
		fprintf(stderr, "Unable to open file '%s'.\n", filename);
		exit(1);
	}
	if(!fgets(buff, sizeof(buff), fp)) {			// read image format
		perror(filename);
		exit(1);
	}
	if(buff[0]!='P' || buff[1]!='6') {				// check the image format
		fprintf(stderr, "Invalid image format (must be 'P6').\n");
		exit(1);
	}
	img = (PPMImage*) malloc(sizeof(PPMImage));		// allocate memory for image
	if(!img) {
		fprintf(stderr, "Unable to allocate memory for image.\n");
		exit(1);
	}
	char c = getc(fp);
	while(c == '#') {								// check for comments, which start with '#'
		while(getc(fp) != '\n');
		c = getc(fp);
	}
	ungetc(c,fp);
	if(fscanf(fp, "%d %d", &img->width, &img->height) != 2) {		// read image size information
		fprintf(stderr, "Invalid image size (error loading '%s').\n", filename);
		exit(1);
	}
	if(fscanf(fp, "%d", &rgb_comp_color) != 1) {					// read RGB component
		fprintf(stderr, "Invalid RGB component (error loading '%s').\n", filename);
		exit(1);
	}
	if(rgb_comp_color != RGB_COMPONENT_COLOR) {						// check RGB component depth
		fprintf(stderr, "'%s' does not have 8-bit components.\n", filename);
		exit(1);
	}
	while(fgetc(fp) != '\n');
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));	// memory allocation for pixel data
	if(!img) {
		fprintf(stderr, "Unable to allocate memory for image.\n");
		exit(1);
	}
	if(fread(img->data, 3*img->width, img->height, fp) != img->height) {			// read pixel data from file
		fprintf(stderr, "Error loading image '%s'.\n", filename);
		exit(1);
	}
	fclose(fp);
	return img;
}

// writes the stored pixel data as a PPM file in the given directory "filename"
void writePPM(const char *filename, PPMImage *img) {
	if(!strstr(filename, ".ppm")) {
		fprintf(stderr, "Invalid file extension (must be '.ppm').\n");
		exit(1);
	}
	FILE *fp;
	fp = fopen(filename, "wb");		// open file for output
	if(!fp) {
		fprintf(stderr, "Unable to open file '%s'.\n",filename);
		exit(1);
	}
	fprintf(fp, "P6\n");									// image format
	fprintf(fp, "# Created by %s\n", CREATOR);				// comments
	fprintf(fp, "%d %d\n", img->width, img->height);		// image size
	fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);				// RGB component depth
	fwrite(img->data, 3*img->width, img->height, fp);		// pixel data
	fclose(fp);
}

// constructor for the PPMImage
PPMImage* createPPMImage(unsigned int image_width, unsigned int image_height, PPMPixel background_color) {
	PPMImage *img = (PPMImage*) malloc(sizeof(PPMImage));
	if(!img) {
		fprintf(stderr, "Unable to allocate memory for image.\n");
		exit(1);
	}
	img->width = image_width;
	img->height = image_height;
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));
	if(!img) {
		fprintf(stderr, "Unable to allocate memory for image.\n");
		exit(1);
	}
	unsigned int i;
	for(i=0;i<img->width*img->height;i++)
		img->data[i] = background_color;
	return img;
}

// deletes the stored data in the pointer (i.e. deallocates memory)
void deletePPMImage(PPMImage *img) {
	free(img->data);
	free(img);
}

// copies the "image2" data to "image1"
void copyPPMImage(PPMImage *image1, PPMImage *image2) {
	image1->width = image2->width;
	image1->height = image2->height;
	image1->data = (PPMPixel*) malloc(image1->width * image1->height * sizeof(PPMPixel));
	memcpy(image1->data, image2->data, image1->width * image1->height * sizeof(PPMPixel));
}

// colors the pixel in the given x and y coordinates where the origin is the center of the image
void putPixel(int x, int y, PPMImage *img, PPMPixel color) {
	if(!(-(signed int)img->width/2<x && x<(signed int)img->width/2 && -(signed int)img->height/2<y && y<(signed int)img->height/2)) {
		fprintf(stderr, "Cannot push pixel, coordinates are out of boundaries.\n");
		exit(1);
	}
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	img->data[center_index + x - img->width*y - 1] = color;
}

// checks if the given two pixels have the same color
unsigned int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2) {
	return pixel1.red == pixel2.red && pixel1.green == pixel2.green && pixel1.blue == pixel2.blue;
}

// keeps the color component of the RGB between 0-255
double truncateRGB(double rgb) {
	return RGB_COMPONENT_COLOR<rgb ? RGB_COMPONENT_COLOR : (rgb<0 ? 0 : rgb);
}

// converts the HSV (hue, saturation, value) color model of a pixel to RGB (red, green, blue)
double* convertHSVtoRGB(double h, double s, double v) {
	const double c = v*s;
	const double x = c*(1 - fabs(fmod(h/60, 2) - 1));
	const double m = v - c;
	double* RGB = (double*) malloc(3*sizeof(double));
	if(h < 60)
		RGB[0] = c, RGB[1] = x, RGB[2] = 0;
	else if(h < 120)
		RGB[0] = x, RGB[1] = c, RGB[2] = 0;
	else if(h < 180)
		RGB[0] = 0, RGB[1] = c, RGB[2] = x;
	else if(h < 240)
		RGB[0] = 0, RGB[1] = x, RGB[2] = c;
	else if(h < 300)
		RGB[0] = x, RGB[1] = 0, RGB[2] = c;
	else
		RGB[0] = c, RGB[1] = 0, RGB[2] = x;
	unsigned short i;
	for(i=0;i<3;i++)
		RGB[i] = (RGB[i] + m)*255;
	return RGB;
}

// converts the HSL (hue, saturation, lightness) color model of a pixel to RGB (red, green, blue)
double* convertHSLtoRGB(double h, double s, double l) {
	const double c = s*(1 - fabs(2*l - 1));
	const double x = c*(1 - fabs(fmod(h/60, 2) - 1));
	const double m = l - c/2;
	double* RGB = (double*) malloc(3*sizeof(double));
	if(h<60)
		RGB[0] = c, RGB[1] = x, RGB[2] = 0;
	else if(h<120)
		RGB[0] = x, RGB[1] = c, RGB[2] = 0;
	else if(h<180)
		RGB[0] = 0, RGB[1] = c, RGB[2] = x;
	else if(h<240)
		RGB[0] = 0, RGB[1] = x, RGB[2] = c;
	else if(h<300)
		RGB[0] = x, RGB[1] = 0, RGB[2] = c;
	else
		RGB[0] = c, RGB[1] = 0, RGB[2] = x;
	unsigned short i;
	for(i=0;i<3;i++)
		RGB[i] = (RGB[i] + m)*255;
	return RGB;
}

// converts the RGB color model of a pixel to HSV
double* convertRGBtoHSV(double r, double g, double b) {
	r /= 255, g /= 255, b /= 255;
	const double maxColor = r<g ? (g<b ? b : g) : (r<b ? b : r);
	const double minColor = r<g ? (r<b ? r : b) : (g<b ? g : b);
	const double d = maxColor - minColor;
	double* HSV = (double*) malloc(3*sizeof(double));
	HSV[2] = maxColor;
	if(maxColor != 0)
		HSV[1] = d/maxColor;
	else
		HSV[1] = 0;
	if(d == 0)
		HSV[0] = 0;
	else if(maxColor == r)
		HSV[0] = 60*fmod((g - b)/d, 6);
	else if(maxColor == g)
		HSV[0] = 60*(2 + (b - r)/d);
	else
		HSV[0] = 60*(4 + (r - g)/d);
	return HSV;
}

// converts the RGB color model of a pixel to HSL
double* convertRGBtoHSL(double r, double g, double b) {
	r /= 255, g /= 255, b /= 255;
	const double maxColor = r<g ? (g<b ? b : g) : (r<b ? b : r);
	const double minColor = r<g ? (r<b ? r : b) : (g<b ? g : b);
	const double d = maxColor - minColor;
	double* HSL = (double*) malloc(3*sizeof(double));
	HSL[2] = (minColor + maxColor)/2;
	HSL[1] = d / (1 - fabs(2*HSL[2] - 1));
	if(d == 0)
		HSL[0] = 0;
	else if(maxColor == r)
		HSL[0] = 60*fmod((g - b)/d, 6);
	else if(maxColor == g)
		HSL[0] = 60*(2 + (b - r)/d);
	else
		HSL[0] = 60*(4 + (r - g)/d);
	return HSL;
}

#endif
