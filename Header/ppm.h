#ifndef _PPM_H_
#define _PPM_H_

#include "functions.h"

static PPMImage* readPPM(const char *filename) {
	char buff[16];
	PPMImage *img;
	FILE *fp;
	int rgb_comp_color;
	fp=fopen(filename,"rb");
	if(!fp) {
		fprintf(stderr,"Unable to open file '%s'.\n",filename);
		exit(1);
	}
	if(!fgets(buff,sizeof(buff),fp)) {
		perror(filename);
		exit(1);
	}
	if(buff[0]!='P' || buff[1]!='6') {
		fprintf(stderr,"Invalid image format (must be 'P6').\n");
		exit(1);
	}
	img = (PPMImage*) malloc(sizeof(PPMImage));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory for image.\n");
		exit(1);
	}
	char c=getc(fp);
	while(c=='#') {
		while(getc(fp)!='\n');
		c=getc(fp);
	}
	ungetc(c,fp);
	if(fscanf(fp,"%d %d",&img->width,&img->height)!=2) {
		fprintf(stderr,"Invalid image size (error loading '%s').\n",filename);
		exit(1);
	}
	if(fscanf(fp,"%d",&rgb_comp_color)!=1) {
		fprintf(stderr,"Invalid RGB component (error loading '%s').\n",filename);
		exit(1);
	}
	if(rgb_comp_color!=RGB_COMPONENT_COLOR) {
		fprintf(stderr,"'%s' does not have 8-bit components.\n",filename);
		exit(1);
	}
	while(fgetc(fp)!='\n');
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory for image.\n");
		exit(1);
	}
	if(fread(img->data, 3*img->width, img->height, fp) != img->height) {
		fprintf(stderr,"Error loading image '%s'.\n",filename);
		exit(1);
	}
	fclose(fp);
	return img;
}

void writePPM(const char *filename, PPMImage *img) {
	if(!strstr(filename,".ppm")) {
		fprintf(stderr,"Invalid file extension (must be '.ppm').\n");
		exit(1);
	}
	FILE *fp;
	fp=fopen(filename,"wb");
	if(!fp) {
		fprintf(stderr,"Unable to open file '%s'.\n",filename);
		exit(1);
	}
	fprintf(fp,"P6\n");
	fprintf(fp,"# Created by %s\n",CREATOR);
	fprintf(fp,"%d %d\n",img->width,img->height);
	fprintf(fp,"%d\n",RGB_COMPONENT_COLOR);
	fwrite(img->data, 3*img->width, img->height, fp);
	fclose(fp);
}

static PPMImage* createPPMImage(unsigned int image_width, unsigned int image_height, PPMPixel background_color) {
	PPMImage *img = (PPMImage*) malloc(sizeof(PPMImage));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory for image.\n");
		exit(1);
	}
	img->width=image_width;
	img->height=image_height;
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory for image.\n");
		exit(1);
	}
	unsigned int i;
	for(i=0;i<img->width*img->height;i++)
		img->data[i] = background_color;
	return img;
}

void copyPPMImage(PPMImage *image1, PPMImage *image2) {
	image1->width=image2->width;
	image1->height=image2->height;
	image1->data = (PPMPixel*) malloc(image1->width * image1->height * sizeof(PPMPixel));
	memcpy(image1->data, image2->data, image1->width * image1->height * sizeof(PPMPixel));
}

void putPixel(int x, int y, PPMImage *img, PPMPixel color) {
	if(!(-(signed int)img->width/2<x && x<(signed int)img->width/2 && -(signed int)img->height/2<y && y<(signed int)img->height/2)) {
		fprintf(stderr,"Cannot push pixel, coordinates are out of boundaries.\n");
		exit(1);
	}
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	img->data[center_index + x - img->width*y - 1] = color;
}

int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2) {
	if(pixel1.red==pixel2.red && pixel1.green==pixel2.green && pixel1.blue==pixel2.blue)
		return 1;
	return 0;
}

double truncateRGB(double rgb) {
	return 255<rgb ? 255 : (rgb<0 ? 0 : rgb);
}

double* convertHSVtoRGB(double h, double s, double v) {
	const double c = v*s;
	const double x = c*(1 - fabs(fmod(h/60,2) - 1));
	const double m = v-c;
	double* RGB=(double*) malloc(3*sizeof(double));
	if(h<60)
		RGB[0]=c, RGB[1]=x, RGB[2]=0;
	else if(h<120)
		RGB[0]=x, RGB[1]=c, RGB[2]=0;
	else if(h<180)
		RGB[0]=0, RGB[1]=c, RGB[2]=x;
	else if(h<240)
		RGB[0]=0, RGB[1]=x, RGB[2]=c;
	else if(h<300)
		RGB[0]=x, RGB[1]=0, RGB[2]=c;
	else
		RGB[0]=c, RGB[1]=0, RGB[2]=x;
	unsigned short i;
	for(i=0;i<3;i++)
		RGB[i]=(RGB[i]+m)*255;
	return RGB;
}

double* convertHSLtoRGB(double h, double s, double l) {
	const double c = s*(1 - fabs(2*l-1));
	const double x = c*(1 - fabs(fmod(h/60,2) - 1));
	const double m = l-c/2;
	double* RGB=(double*) malloc(3*sizeof(double));
	if(h<60)
		RGB[0]=c, RGB[1]=x, RGB[2]=0;
	else if(h<120)
		RGB[0]=x, RGB[1]=c, RGB[2]=0;
	else if(h<180)
		RGB[0]=0, RGB[1]=c, RGB[2]=x;
	else if(h<240)
		RGB[0]=0, RGB[1]=x, RGB[2]=c;
	else if(h<300)
		RGB[0]=x, RGB[1]=0, RGB[2]=c;
	else
		RGB[0]=c, RGB[1]=0, RGB[2]=x;
	unsigned short i;
	for(i=0;i<3;i++)
		RGB[i]=(RGB[i]+m)*255;
	return RGB;
}

double* convertRGBtoHSV(double r, double g, double b) {
	r/=255, g/=255, b/=255;
	const double maxColor = r<g ? (g<b ? b : g) : (r<b ? b : r);
	const double minColor = r<g ? (r<b ? r : b) : (g<b ? g : b);
	const double d = maxColor - minColor;
	double* HSV=(double*) malloc(3*sizeof(double));
	HSV[2] = maxColor;
	if(maxColor!=0)
		HSV[1] = d/maxColor;
	else
		HSV[1]=0;
	if(d==0)
		HSV[0]=0;
	else if(maxColor==r)
		HSV[0] = 60*fmod((g-b)/d,6);
	else if(maxColor==g)
		HSV[0] = 60*(2 + (b-r)/d);
	else
		HSV[0] = 60*(4 + (r-g)/d);
	return HSV;
}

double* convertRGBtoHSL(double r, double g, double b) {
	r/=255, g/=255, b/=255;
	const double maxColor = r<g ? (g<b ? b : g) : (r<b ? b : r);
	const double minColor = r<g ? (r<b ? r : b) : (g<b ? g : b);
	const double d = maxColor - minColor;
	double* HSL=(double*) malloc(3*sizeof(double));
	HSL[2] = (minColor + maxColor)/2;
	HSL[1] = d / (1 - fabs(2*HSL[2]-1));
	if(d==0)
		HSL[0]=0;
	else if(maxColor==r)
		HSL[0] = 60*fmod((g-b)/d,6);
	else if(maxColor==g)
		HSL[0] = 60*(2 + (b-r)/d);
	else
		HSL[0] = 60*(4 + (r-g)/d);
	return HSL;
}

#endif
