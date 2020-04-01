#ifndef _FRACTALS_H_
#define _FRACTALS_H_

#include "functions.h"
#include "ppm.h"
#include "patterns.h"

static PPMImage* print8bitMandelbrotSet(unsigned int image_width, unsigned int image_height) {
	PPMImage *img=createPPMImage(image_width,image_height,BLACK);
	unsigned int index=0,counter=0;
	float i,j,r,x,y;
	PPMPixel pixel_Array[16] = { BLACK, MAROON, RED, ORANGE,
								 YELLOW, CHARTREUSE, GREEN, SPRING_GREEN,
								 CYAN, AZURE, BLUE, VIOLET,
								 MAGENTA, ROSE, MAGENTA, ROSE };
	for(y=-(signed int)image_height/2 - 1;y<image_height/2;y++)
		for(x=0;x<image_width;x++) {
			img->data[counter++]=pixel_Array[index&15];
			for(i=index=r=0; j=r*r-i*i-2+x/25, i=2*r*i+y/10, j*j+i*i<11 && index++<111;r=j);
		}
	return img;
}

static PPMImage* printEpicMandelbrotSet(unsigned int image_width, unsigned int image_height) {
	PPMImage *img=createPPMImage(image_width,image_height,BLACK);
	unsigned int iX,iY,iterator,counter=0;
	const unsigned int iterationMax=200;
	const double MinRe=-2.5, MaxRe=1.5, MinIm=-2.0, escapeRadius=2;
	const double MaxIm = MinIm + (MaxRe-MinRe)*image_height/image_width;
	const double Re_factor = (MaxRe-MinRe)/image_width;
	const double Im_factor = (MaxIm-MinIm)/image_height;
	const double ER2=escapeRadius*escapeRadius;
	for(iY=0;iY<image_height;iY++) {
		double c_im = MaxIm - iY*Im_factor;
		for(iX=0;iX<image_width;iX++) {
			double c_re = MinRe + iX*Re_factor;
			double Z_re=c_re, Z_im=c_im;
			for(iterator=0;iterator<iterationMax;iterator++) {
				double Z_re2=Z_re*Z_re, Z_im2=Z_im*Z_im;
				if(Z_re2+Z_im2>ER2)
					break;
				Z_im = 2*Z_re*Z_im + c_im;
            	Z_re = Z_re2 - Z_im2 + c_re;
			}
			double x = (double)iterator / iterationMax * RGB_COMPONENT_COLOR;
			if(x<RGB_COMPONENT_COLOR/2) {
				img->data[counter].red = 15*x/8 + 16;
				img->data[counter].green = 0;
				img->data[counter].blue = 0;
			}
			else if(x<RGB_COMPONENT_COLOR) {
				img->data[counter].red = RGB_COMPONENT_COLOR;
				img->data[counter].green = 2*x - RGB_COMPONENT_COLOR;
				img->data[counter].blue = 2*x - RGB_COMPONENT_COLOR;
			}
			counter++;
		}
	}
	return img;
}

void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img, PPMPixel color) {
	Point p3,p4,p5;
	double theta=M_PI/3;
	p3 = (Point){(2*p1.x+p2.x)/3, (2*p1.y+p2.y)/3};
	p4 = (Point){(2*p2.x+p1.x)/3, (2*p2.y+p1.y)/3};
	p5 = (Point){p3.x + (p4.x-p3.x)*cos(theta) + (p4.y-p3.y)*sin(theta), p3.y - (p4.x-p3.x)*sin(theta) + (p4.y-p3.y)*cos(theta)};
	if(iteration>0) {
		drawKochCurve(p1,p3,iteration-1,img,color);
		drawKochCurve(p3,p5,iteration-1,img,color);
		drawKochCurve(p5,p4,iteration-1,img,color);
		drawKochCurve(p4,p2,iteration-1,img,color);
	}
	else {
		drawLine(p1.x,p1.y,p3.x,p3.y,img,color);
		drawLine(p3.x,p3.y,p5.x,p5.y,img,color);
		drawLine(p5.x,p5.y,p4.x,p4.y,img,color);
		drawLine(p4.x,p4.y,p2.x,p2.y,img,color);
	}
}

void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img, PPMPixel color) {
	Point p1 = (Point){x1,y1};
	Point p2 = (Point){x2,y2};
	drawKochCurve(p1,p2,iteration,img,color);
}

void drawTriangles(double x, double y, double h, PPMImage *img, PPMPixel color) {
	float delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x - (h+delta)/sqrt(3), y - (h+delta)/3, x + (h+delta)/sqrt(3), y - (h+delta)/3, img, color);
		drawLine(x - (h+delta)/sqrt(3), y - (h+delta)/3, x, y + 2*(h+delta)/3, img, color);
		drawLine(x, y + 2*(h+delta)/3, x + (h+delta)/sqrt(3), y - (h+delta)/3, img, color);
	}
}

void drawTrianglesv2(double x, double y, double h, PPMImage *img, PPMPixel color) {
	float delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x - (h+delta)/sqrt(3), y + (h+delta)/3, x + (h+delta)/sqrt(3), y + (h+delta)/3, img, color);
		drawLine(x - (h+delta)/sqrt(3), y + (h+delta)/3, x, y - 2*(h+delta)/3, img, color);
		drawLine(x, y - 2*(h+delta)/3, x + (h+delta)/sqrt(3), y + (h+delta)/3, img, color);
	}
}

void printSierpinski(double x, double y, double h, PPMImage *img, PPMPixel color) {
	if(h<5)
		return;
	if(-(signed int)img->width/2<x && -(signed int)img->width/2<y && x<img->width/2 && y<img->height/2)
		drawTriangles(x,y,h,img,color);
	printSierpinski(x, y - 2*h/3, h/2, img, color);
	printSierpinski(x - h/sqrt(3), y+h/3, h/2, img, color);
	printSierpinski(x + h/sqrt(3), y+h/3, h/2, img, color);
}

static PPMImage* printJulia(int image_width, int image_height, int zoom) {
	PPMImage *img = createPPMImage((unsigned int)image_width,(unsigned int)image_height,BLACK);
	long double cX=-0.7, cY=0.27015, moveX=0.0, moveY=0.0;
	const unsigned int iterationMax=255;
	int x,y;
	for(x=0;x<image_width;x++)
		for(y=0;y<image_height;y++) {
			long double zx = 1.5*(x - image_width/2)/(0.5*zoom*image_width) + moveX;
			long double zy = 1.0*(y - image_height/2)/(0.5*zoom*image_height) + moveY;
			unsigned int i = iterationMax;
			for(i=iterationMax;zx*zx+zy*zy<4 && i>0;i--) {
				long double temp = zx*zx - zy*zy + cX;
				zy = 2.0*zx*zy + cY;
				zx = temp;
			}
			double *RGB=convertHSVtoRGB((double)i / iterationMax * 360, 1, i>1 ? 1 : 0);
			img->data[img->width*(img->height-y-1) + x].red = RGB[0];
			img->data[img->width*(img->height-y-1) + x].green = RGB[1];
			img->data[img->width*(img->height-y-1) + x].blue = RGB[2];
		}
	return img;
}

void printYinYangFractal(int xc, int yc, unsigned int radius, double smaller_radius_coefficient, unsigned int iteration, PPMPixel color1, PPMPixel color2, PPMImage *img) {
	if(iteration==0) {
		printMultipleCircles(xc,yc,radius,0,1,img,color1);
		return;
	}
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	printMultipleCircles(xc, yc, radius, 0, 1, img, PURPLE);
	printMultipleCircles(xc, yc+radius/2, radius/2, 0, 1, img, PURPLE);
	printMultipleCircles(xc, yc-radius/2, radius/2, 0, 1, img, PURPLE);
	fillRegion(xc-radius/2, yc, color2, img->data[center_index + (xc-radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc-radius/2, color2, img->data[center_index + xc - img->width*(yc-radius/2) - 1], img);
	fillRegion(xc+radius/2, yc, color1, img->data[center_index + (xc+radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc+radius/2, color1, img->data[center_index + xc - img->width*(yc+radius/2) - 1], img);
	printMultipleCircles(xc, yc+radius/2, (unsigned int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc+radius/2, color2, color1, img);
	printMultipleCircles(xc, yc-radius/2, (unsigned int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc-radius/2, color1, color2, img);
	printMultipleCircles(xc, yc-radius/2, radius/2, 0, 1, img, color2);
	printYinYangFractal(xc, yc+radius/2, (unsigned int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);
	printYinYangFractal(xc, yc-radius/2, (unsigned int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);
	printMultipleCircles(xc, yc, radius, 0, 1, img, color1);
	printMultipleCircles(xc, yc+radius/2, radius/2, 0, 1, img, color1);
}

void printCircleFractal_xAxis(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color) {
	printMultipleCircles(xc,yc,radius,0,1,img,color);
	if(radius>2) {
		printCircleFractal_xAxis(xc+radius/2, yc, radius/2, img, color);
		printCircleFractal_xAxis(xc-radius/2, yc, radius/2, img, color);
	}
}

void printCircleFractal_xyPlane(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color) {
	printMultipleCircles(xc,yc,radius,0,1,img,color);
	if(radius>8) {
		printCircleFractal_xyPlane(xc+radius/2, yc, radius/2, img, color);
		printCircleFractal_xyPlane(xc-radius/2, yc, radius/2, img, color);
		printCircleFractal_xyPlane(xc, yc+radius/2, radius/2, img, color);
		printCircleFractal_xyPlane(xc, yc-radius/2, radius/2, img, color);
	}
}

void printTree(int x1, int y1, int main_angle, int branch_angle, unsigned int iteration, PPMImage *img, PPMPixel color) {
	if(iteration==0)
		return;
	int x2 = x1 - (int)(cos(main_angle*M_PI/180)*iteration*10.0);
	int y2 = y1 - (int)(sin(main_angle*M_PI/180)*iteration*10.0);
	drawLine(x1,y1,x2,y2,img,color);
	printTree(x2, y2, main_angle-branch_angle, branch_angle, iteration-1, img, color);
	printTree(x2, y2, main_angle+branch_angle, branch_angle, iteration-1, img, color);
}

void printCantor(int x, int y, unsigned int length, PPMImage *img, PPMPixel color) {
	if(length>=1) {
		drawLine(x,y,x+length,y,img,color);
		y-=20;
		printCantor(x, y, length/3, img, color);
		printCantor(x+2*length/3, y, length/3, img, color);
	}
}

#endif
