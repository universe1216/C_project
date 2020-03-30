#ifndef _FRACTALS_H_
#define _FRACTALS_H_

#include "functions.h"
#include "ppm.h"
#include "patterns.h"

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

#endif
