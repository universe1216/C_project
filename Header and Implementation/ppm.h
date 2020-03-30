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
		fprintf(stderr,"Unable to allocate memory.\n");
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
		fprintf(stderr,"Unable to allocate memory.\n");
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

static PPMImage* createPPMImage(unsigned int width, unsigned int height, PPMPixel color) {
	PPMImage *img = (PPMImage*) malloc(sizeof(PPMImage));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(1);
	}
	img->width=width;
	img->height=height;
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));
	if(!img) {
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(1);
	}
	unsigned int i;
	for(i=0;i<width*height;i++)
		img->data[i] = color;
	return img;
}

int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2) {
	if(pixel1.red==pixel2.red && pixel1.green==pixel2.green && pixel1.blue==pixel2.blue)
		return 1;
	return 0;
}

double* convertHSVtoRGB(double h, double s, double v) {
	const double c = v*s;
	const double x = c*(1 - abs((((int)h/60)%2) - 1));
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

#endif
