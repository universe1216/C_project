#ifndef _PPM_FUNCTIONS_H_
#define _PPM_FUNCTIONS_H_

#include "ppm structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CREATOR "DasDasAras"
#define RGB_COMPONENT_COLOR 255
#define max(a,b) a>b ? a : b

const PPMPixel RED		=	(PPMPixel){255,0,0};
const PPMPixel GREEN	=	(PPMPixel){0,255,0};
const PPMPixel BLUE		=	(PPMPixel){0,0,255};
const PPMPixel YELLOW	=	(PPMPixel){255,255,0};
const PPMPixel PURPLE	=	(PPMPixel){128,0,128};
const PPMPixel BLACK	=	(PPMPixel){0,0,0};
const PPMPixel WHITE	=	(PPMPixel){255,255,255};

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
				img->data[counter++] = (PPMPixel){255,0,0};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){255,127,0};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){255,255,0};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){0,255,0};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){0,0,255};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){75,0,130};
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){148,0,211};
	}
	return img;
}

static PPMImage* print8bitMandelbrotSet(unsigned int width, unsigned int height) {
	PPMImage *img = createPPMImage(width,height,BLACK);
	unsigned int index=0,counter=0;
	float i,j,r,x,y;
	PPMPixel pixel_Array[16] = { {0,0,0}, {175,0,0}, {255,0,0}, {255,127,0},
								 {255,255,0}, {127,255,0}, {0,255,0}, {0,255,127},
								 {0,255,255}, {0,127,255}, {0,0,255}, {127,0,255},
								 {255,0,255}, {255,0,127}, {255,0,255}, {255,0,127} };
	for(y=(signed int)-height/2 - 1;y<height/2;y++)
		for(x=0;x<width;x++) {
			img->data[counter++]=pixel_Array[index&15];
			for(i=index=r=0; j=r*r-i*i-2+x/25, i=2*r*i+y/10, j*j+i*i<11 && index++<111;r=j);
		}
	return img;
}

static PPMImage* printEpicMandelbrotSet(unsigned int width, unsigned int height) {
	PPMImage *img = createPPMImage(width,height,BLACK);
	unsigned int iX, iY, Iteration, counter=0;
	const unsigned int IterationMax=200;
	const double CxMin=-2.5, CxMax=1.5, CyMin=-2.0, CyMax=2.0, EscapeRadius=2;
	double PixelWidth=(CxMax-CxMin)/width;
	double PixelHeight=(CyMax-CyMin)/height;
	double ER2=EscapeRadius*EscapeRadius;
	double Cx, Cy, Zx, Zy, Zx2, Zy2;
	for(iY=0;iY<height;iY++) {
		Cy = CyMin + iY*PixelHeight;
		if(fabs(Cy) < PixelHeight/2)
			Cy=0.0;
		for(iX=0;iX<width;iX++) {         
			Cx = CxMin + iX*PixelWidth;
			Zx = Zy = 0.0;
			Zx2=Zx*Zx;
			Zy2=Zy*Zy;
			for(Iteration=0;Iteration<IterationMax && Zx2+Zy2<ER2;Iteration++) {
				Zy=2*Zx*Zy + Cy;
				Zx=Zx2-Zy2+Cx;
				Zx2=Zx*Zx;
				Zy2=Zy*Zy;
			}
			img->data[counter].red = img->data[counter].green = img->data[counter].blue = Iteration==IterationMax ? 0 : 255;
			counter++;
		}
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
			img->data[counter++] = (PPMPixel){0,0,0};
		for(j=1;j<=i;j++)
			img->data[counter++] = (PPMPixel){255,0,0};
		for(j=1;j<=size-i;j++)
			img->data[counter++] = (PPMPixel){0,0,0};
		for(j=1;j<=i;j++)
			img->data[counter++] = (PPMPixel){255,0,0};
		for(j=1;j<size-i;j+=2)
			img->data[counter++] = (PPMPixel){0,0,0};
	}
	for(i=size;i>=1;i--) {
		for(j=i;j<size;j++)
			img->data[counter++] = (PPMPixel){0,0,0};
		for(j=1;j<=2*i-1;j++)
			img->data[counter++] = (PPMPixel){255,0,0};
		for(j = size%4==1 || size%4==2 ? i : i-1;j<size;j++)
			img->data[counter++] = (PPMPixel){0,0,0};
	}
	return img;
}

void drawLine(int x1, int y1, int x2, int y2, PPMImage *img, PPMPixel line_color) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	int dx=x2-x1, dy=y2-y1;
	unsigned int steps = abs(dx)>abs(dy) ? abs(dx) : abs(dy);
	float Xinc = dx/(float)steps, Yinc = dy/(float)steps, x=x1, y=y1;
	unsigned int i;
	for(i=0; i<=steps; i++, x+=Xinc, y+=Yinc)
		img->data[center_index + (int)x - img->width*(int)y - 1] = line_color;
}

void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img) {
	Point p3,p4,p5;
	double theta = M_PI/3;
	p3 = (Point){(2*p1.x+p2.x)/3, (2*p1.y+p2.y)/3};
	p4 = (Point){(2*p2.x+p1.x)/3, (2*p2.y+p1.y)/3};
	p5 = (Point){p3.x + (p4.x-p3.x)*cos(theta) + (p4.y-p3.y)*sin(theta), p3.y - (p4.x-p3.x)*sin(theta) + (p4.y-p3.y)*cos(theta)};
	if(iteration>0) {
		drawKochCurve(p1,p3,iteration-1,img);
		drawKochCurve(p3,p5,iteration-1,img);
		drawKochCurve(p5,p4,iteration-1,img);
		drawKochCurve(p4,p2,iteration-1,img);
	}
	else {
		PPMPixel RED = (PPMPixel){255,0,0};
		drawLine(p1.x,p1.y,p3.x,p3.y,img,RED);
		drawLine(p3.x,p3.y,p5.x,p5.y,img,RED);
		drawLine(p5.x,p5.y,p4.x,p4.y,img,RED);
		drawLine(p4.x,p4.y,p2.x,p2.y,img,RED);
	}
}

void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img) {
	Point p1 = (Point){x1,y1};
	Point p2 = (Point){x2,y2};
	drawKochCurve(p1,p2,iteration,img);
}

int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2) {
	if(pixel1.red==pixel2.red && pixel1.green==pixel2.green && pixel1.blue==pixel2.blue)
		return 1;
	return 0;
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

void drawTriangles(float x, float y, float h, PPMImage *img) {
	float delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x-(h+delta)/sqrt(3), y-(h+delta)/3, x+(h+delta)/sqrt(3), y-(h+delta)/3, img, RED);
		drawLine(x-(h+delta)/sqrt(3), y-(h+delta)/3, x, y+2*(h+delta)/3, img, RED);
		drawLine(x, y+2*(h+delta)/3, x+(h+delta)/sqrt(3), y-(h+delta)/3, img, RED);
	}
}

void drawTrianglesv2(float x, float y, float h, PPMImage *img) {
	float delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x-(h+delta)/sqrt(3), y+(h+delta)/3, x+(h+delta)/sqrt(3), y+(h+delta)/3, img, RED);
		drawLine(x-(h+delta)/sqrt(3), y+(h+delta)/3, x, y-2*(h+delta)/3, img, RED);
		drawLine(x, y-2*(h+delta)/3, x+(h+delta)/sqrt(3), y+(h+delta)/3, img, RED);
	}
}

void printSierpinski(float x, float y, float h, PPMImage *img) {
	if(h<5)
		return;
	if(-(signed int)img->width/2<x && -(signed int)img->width/2<y && x<img->width/2 && y<img->height/2)
		drawTriangles(x,y,h,img);
	printSierpinski(x, y-2*h/3, h/2, img);
	printSierpinski(x-h/sqrt(3), y+h/3, h/2, img);
	printSierpinski(x+h/sqrt(3), y+h/3, h/2, img);
}

static PPMImage* printJulia(int width, int height, int zoom) {
	PPMImage *img = createPPMImage(width,height,BLACK);
	long double cX=-0.7, cY=0.27015, moveX=0.0, moveY=0.0;
	const unsigned int maxIter=255;
	int x,y;
	for(x=0;x<width;x++)
		for(y=0;y<height;y++) {
			long double zx = 1.5*(x - width/2)/(0.5*zoom*width) + moveX;
			long double zy = 1.0*(y - height/2)/(0.5*zoom*height) + moveY;
			unsigned int i = maxIter;
			for(i=maxIter;zx*zx+zy*zy<4 && i>0;i--) {
				long double temp = zx*zx - zy*zy + cX;
				zy = 2.0*zx*zy + cY;
				zx = temp;
			}
			double *RGB=convertHSVtoRGB((double)i / maxIter * 360, 1, i>1 ? 1 : 0);
			img->data[img->width*(img->height-1) + x - img->width*y].red = RGB[0];
			img->data[img->width*(img->height-1) + x - img->width*y].green = RGB[1];
			img->data[img->width*(img->height-1) + x - img->width*y].blue = RGB[2];
		}
	return img;
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
