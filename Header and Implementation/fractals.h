#ifndef _FRACTALS_H_
#define _FRACTALS_H_

#include "functions.h"
#include "ppm.h"
#include "patterns.h"

static PPMImage* print8bitMandelbrotSet(unsigned int width, unsigned int height) {
	PPMImage *img = createPPMImage(width,height,BLACK);
	unsigned int index=0,counter=0;
	float i,j,r,x,y;
	PPMPixel pixel_Array[16] = { BLACK, MAROON, RED, ORANGE,
								 YELLOW, CHARTREUSE, GREEN, SPRING_GREEN,
								 CYAN, AZURE, BLUE, VIOLET,
								 MAGENTA, ROSE, MAGENTA, ROSE };
	for(y=(signed int)-height/2 - 1;y<height/2;y++)
		for(x=0;x<width;x++) {
			img->data[counter++]=pixel_Array[index&15];
			for(i=index=r=0; j=r*r-i*i-2+x/25, i=2*r*i+y/10, j*j+i*i<11 && index++<111;r=j);
		}
	return img;
}

static PPMImage* printEpicMandelbrotSet(unsigned int width, unsigned int height) {
	PPMImage *img = createPPMImage(width,height,BLACK);
	unsigned int iX, iY, iterator, counter=0;
	const unsigned int iterationMax=200;
	const double CxMin=-2.5, CxMax=1.5, CyMin=-2.0, CyMax=2.0, EscapeRadius=2;
	double PixelWidth=(CxMax-CxMin)/width;
	double PixelHeight=(CyMax-CyMin)/height;
	double ER2=EscapeRadius*EscapeRadius;
	double Cx, Cy, Zx, Zy, Zx2, Zy2;
	for(iY=0;iY<height;iY++) {
		Cy = CyMin + iY*PixelHeight;
		if(fabs(Cy)<PixelHeight/2)
			Cy=0.0;
		for(iX=0;iX<width;iX++) {         
			Cx = CxMin + iX*PixelWidth;
			Zx=Zy=0.0;
			Zx2=Zx*Zx;
			Zy2=Zy*Zy;
			for(iterator=0;iterator<iterationMax && Zx2+Zy2<ER2;iterator++) {
				Zy=2*Zx*Zy + Cy;
				Zx=Zx2-Zy2+Cx;
				Zx2=Zx*Zx;
				Zy2=Zy*Zy;
			}
			img->data[counter] = iterator==iterationMax ? BLACK : WHITE;
			counter++;
		}
	}
	return img;
}

void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img, PPMPixel curve_color) {
	Point p3,p4,p5;
	double theta = M_PI/3;
	p3 = (Point){(2*p1.x+p2.x)/3, (2*p1.y+p2.y)/3};
	p4 = (Point){(2*p2.x+p1.x)/3, (2*p2.y+p1.y)/3};
	p5 = (Point){p3.x + (p4.x-p3.x)*cos(theta) + (p4.y-p3.y)*sin(theta), p3.y - (p4.x-p3.x)*sin(theta) + (p4.y-p3.y)*cos(theta)};
	if(iteration>0) {
		drawKochCurve(p1,p3,iteration-1,img,curve_color);
		drawKochCurve(p3,p5,iteration-1,img,curve_color);
		drawKochCurve(p5,p4,iteration-1,img,curve_color);
		drawKochCurve(p4,p2,iteration-1,img,curve_color);
	}
	else {
		drawLine(p1.x,p1.y,p3.x,p3.y,img,curve_color);
		drawLine(p3.x,p3.y,p5.x,p5.y,img,curve_color);
		drawLine(p5.x,p5.y,p4.x,p4.y,img,curve_color);
		drawLine(p4.x,p4.y,p2.x,p2.y,img,curve_color);
	}
}

void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img, PPMPixel curve_color) {
	Point p1 = (Point){x1,y1};
	Point p2 = (Point){x2,y2};
	drawKochCurve(p1,p2,iteration,img,curve_color);
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
	const unsigned int iterationMax=255;
	int x,y;
	for(x=0;x<width;x++)
		for(y=0;y<height;y++) {
			long double zx = 1.5*(x - width/2)/(0.5*zoom*width) + moveX;
			long double zy = 1.0*(y - height/2)/(0.5*zoom*height) + moveY;
			unsigned int i = iterationMax;
			for(i=iterationMax;zx*zx+zy*zy<4 && i>0;i--) {
				long double temp = zx*zx - zy*zy + cX;
				zy = 2.0*zx*zy + cY;
				zx = temp;
			}
			double *RGB=convertHSVtoRGB((double)i / iterationMax * 360, 1, i>1 ? 1 : 0);
			img->data[img->width*(img->height-1) + x - img->width*y].red = RGB[0];
			img->data[img->width*(img->height-1) + x - img->width*y].green = RGB[1];
			img->data[img->width*(img->height-1) + x - img->width*y].blue = RGB[2];
		}
	return img;
}

void printYinYangFractal(int xc, int yc, int radius, double smaller_radius_coefficient, unsigned int iteration, PPMPixel color1, PPMPixel color2, PPMImage *img) {
	if(iteration==0) {
		printCircles(xc, yc, radius, 0, 1, img, color1);
		return;
	}
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	printCircles(xc, yc, radius, 0, 1, img, PURPLE);
	printCircles(xc, yc+radius/2, radius/2, 0, 1, img, PURPLE);
	printCircles(xc, yc-radius/2, radius/2, 0, 1, img, PURPLE);
	fillRegion(xc-radius/2, yc, color2, img->data[center_index + (xc-radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc-radius/2, color2, img->data[center_index + xc - img->width*(yc-radius/2) - 1], img);
	fillRegion(xc+radius/2, yc, color1, img->data[center_index + (xc+radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc+radius/2, color1, img->data[center_index + xc - img->width*(yc+radius/2) - 1], img);
	printCircles(xc, yc+radius/2, (int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc+radius/2, color2, color1, img);
	printCircles(xc, yc-radius/2, (int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc-radius/2, color1, color2, img);
	printCircles(xc, yc-radius/2, radius/2, 0, 1, img, color2);
	printYinYangFractal(xc, yc+radius/2, (int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);
	printYinYangFractal(xc, yc-radius/2, (int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);
	printCircles(xc, yc, radius, 0, 1, img, color1);
	printCircles(xc, yc+radius/2, radius/2, 0, 1, img, color1);
}

void printCircleFractal_xAxis(int x, int y, int radius, PPMImage *img, PPMPixel color) {
	printCircles(x, y, radius, 0, 1, img, color);
	if(radius>2) {
		printCircleFractal_xAxis(x+radius/2, y, radius/2, img, color);
		printCircleFractal_xAxis(x-radius/2, y, radius/2, img, color);
	}
}

void printCircleFractal_xyPlane(int x, int y, int radius, PPMImage *img, PPMPixel color) {
	printCircles(x, y, radius, 0, 1, img, color);
	if(radius>8) {
		printCircleFractal_xyPlane(x+radius/2, y, radius/2, img, color);
		printCircleFractal_xyPlane(x-radius/2, y, radius/2, img, color);
		printCircleFractal_xyPlane(x, y+radius/2, radius/2, img, color);
		printCircleFractal_xyPlane(x, y-radius/2, radius/2, img, color);
	}
}

void printTree(int x1, int y1, int angle, int iteration, PPMImage *img, PPMPixel color) {
	if(iteration==0)
		return;
	int x2 = x1 - (int)(cos(angle*M_PI/180)*iteration*10.0);
	int y2 = y1 - (int)(sin(angle*M_PI/180)*iteration*10.0);
	drawLine(x1,y1,x2,y2,img,color);
	printTree(x2,y2,angle-20,iteration-1,img,color);
	printTree(x2,y2,angle+20,iteration-1,img,color);
}

#endif
