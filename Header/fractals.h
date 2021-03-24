#ifndef _FRACTALS_H_
#define _FRACTALS_H_

#include "shapes.h"

// header file which contains all the fractal drawing functions such as Mandelbrot, Koch, Julia, Barnsley's Fern, etc.

// draws an image of the Mandelbrot set
PPMImage* drawMandelbrotSet(unsigned int image_width, unsigned int image_height) {
	PPMImage *img = createPPMImage(image_width, image_height, BLACK);
	unsigned int iX, iY, iterator, counter = 0;
	const unsigned int iterationMax = 200;
	const double MinRe = -2.5, MaxRe = 1.5, MinIm = -2.0, escapeRadius = 2;
	const double MaxIm = MinIm + (MaxRe - MinRe) * image_height / image_width;
	const double Re_factor = (MaxRe - MinRe) / image_width;
	const double Im_factor = (MaxIm - MinIm) / image_height;
	const double ER2 = escapeRadius*escapeRadius;
	for(iY=0;iY<image_height;iY++) {
		double c_im = MaxIm - iY*Im_factor;
		for(iX=0;iX<image_width;iX++) {
			double c_re = MinRe + iX*Re_factor;
			double Z_re = c_re, Z_im = c_im;
			for(iterator=0;iterator<iterationMax;iterator++) {
				double Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
				if(Z_re2 + Z_im2 > ER2)
					break;
				Z_im = 2*Z_re*Z_im + c_im;
            	Z_re = Z_re2 - Z_im2 + c_re;
			}
			double x = (double)iterator / iterationMax * RGB_COMPONENT_COLOR;
			if(x < RGB_COMPONENT_COLOR/2) {
				img->data[counter].red = 15*x/8 + 16;
				img->data[counter].green = 0;
				img->data[counter].blue = 0;
			}
			else if(x < RGB_COMPONENT_COLOR) {
				img->data[counter].red = RGB_COMPONENT_COLOR;
				img->data[counter].green = 2*x - RGB_COMPONENT_COLOR;
				img->data[counter].blue = 2*x - RGB_COMPONENT_COLOR;
			}
			counter++;
		}
	}
	return img;
}

// a recursive utility function of "drawKochCurve", which is called over and over again to complete the Koch curve
void drawKochCurveUtil(Point p1, Point p2, unsigned int iteration, PPMImage *img, PPMPixel color) {
	const double theta = M_PI/3;
	Point p3 = (Point){(2*p1.x + p2.x)/3, (2*p1.y + p2.y)/3};
	Point p4 = (Point){(2*p2.x + p1.x)/3, (2*p2.y + p1.y)/3};
	Point p5 = (Point){p3.x + (p4.x - p3.x)*cos(theta) + (p4.y - p3.y)*sin(theta), p3.y - (p4.x - p3.x)*sin(theta) + (p4.y - p3.y)*cos(theta)};
	if(iteration > 0) {
		drawKochCurveUtil(p1, p3, iteration-1, img, color);
		drawKochCurveUtil(p3, p5, iteration-1, img, color);
		drawKochCurveUtil(p5, p4, iteration-1, img, color);
		drawKochCurveUtil(p4, p2, iteration-1, img, color);
	}
	else {
		drawLine(p1.x, p1.y, p3.x, p3.y, img, color);
		drawLine(p3.x, p3.y, p5.x, p5.y, img, color);
		drawLine(p5.x, p5.y, p4.x, p4.y, img, color);
		drawLine(p4.x, p4.y, p2.x, p2.y, img, color);
	}
}

// draws the Koch curve on the given image, the curve becomes more and more detailed as the "iteration" number increases
void drawKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img, PPMPixel color) {
	Point p1 = (Point){x1, y1};
	Point p2 = (Point){x2, y2};
	drawKochCurveUtil(p1, p2, iteration, img, color);	// initial call of the utility function
}

// utility function of "drawSierpinski", draws smaller and smaller triangles
void drawSierpinskiUtil1(double x, double y, double h, PPMImage *img, PPMPixel color) {
	double delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x - (h + delta)/sqrt(3), y - (h + delta)/3, x + (h + delta)/sqrt(3), y - (h + delta)/3, img, color);
		drawLine(x - (h + delta)/sqrt(3), y - (h + delta)/3, x, y + 2*(h + delta)/3, img, color);
		drawLine(x, y + 2*(h + delta)/3, x + (h + delta)/sqrt(3), y - (h + delta)/3, img, color);
	}
}

// utility function of "drawSierpinski", draws smaller and smaller upside down triangles
void drawSierpinskiUtil2(double x, double y, double h, PPMImage *img, PPMPixel color) {
	double delta;
	for(delta=0;delta>-5;delta--) {
		drawLine(x - (h + delta)/sqrt(3), y + (h + delta)/3, x + (h+delta)/sqrt(3), y + (h + delta)/3, img, color);
		drawLine(x - (h + delta)/sqrt(3), y + (h + delta)/3, x, y - 2*(h+delta)/3, img, color);
		drawLine(x, y - 2*(h + delta)/3, x + (h + delta)/sqrt(3), y + (h+delta)/3, img, color);
	}
}

// draws the Sierpinski triangle on the given image
void drawSierpinski(double x, double y, double h, PPMImage *img, PPMPixel color) {
	if(h<5)
		return;
	if(-(signed int)img->width/2<x && x<img->width/2 && -(signed int)img->height/2<y && y<img->height/2)
		drawSierpinskiUtil1(x, y, h, img, color);
	drawSierpinski(x, y - 2*h/3, h/2, img, color);
	drawSierpinski(x - h/sqrt(3), y + h/3, h/2, img, color);
	drawSierpinski(x + h/sqrt(3), y + h/3, h/2, img, color);
}

// draws an image of the Julia set
PPMImage* drawJulia(int image_width, int image_height, int zoom) {
	PPMImage *img = createPPMImage((unsigned int)image_width, (unsigned int)image_height, BLACK);
	const double cX = -0.7, cY = 0.27015, moveX = 0.0, moveY = 0.0;
	const unsigned int iterationMax = 255;
	int x,y;
	for(x=0;x<image_width;x++)
		for(y=0;y<image_height;y++) {
			double zx = 1.5*(x - image_width/2)/(0.5*zoom*image_width) + moveX;
			double zy = 1.0*(y - image_height/2)/(0.5*zoom*image_height) + moveY;
			unsigned int i = iterationMax;
			for(i=iterationMax;zx*zx+zy*zy<4 && i>0;i--) {
				double temp = zx*zx - zy*zy + cX;
				zy = 2.0*zx*zy + cY;
				zx = temp;
			}
			double *RGB = convertHSVtoRGB((double)i / iterationMax * 360, 1, i>1 ? 1 : 0);
			img->data[img->width*(img->height-y-1) + x].red = RGB[0];
			img->data[img->width*(img->height-y-1) + x].green = RGB[1];
			img->data[img->width*(img->height-y-1) + x].blue = RGB[2];
		}
	return img;
}

// draws the yin yang fractal on the given image where:
// (xc, yc): x and y coordinates of the center of outermost yin yang
// radius: radius of the outermost yin yang
// smaller_radius_coefficient: ratio of the inner circle radius to outer circle radius in any given yin yang 
// iteration: number of nested yin yangs
// color1, color2: colors of the each half of yin yangs
void drawYinYangFractal(int xc, int yc, unsigned int radius, double smaller_radius_coefficient, unsigned int iteration, PPMPixel color1, PPMPixel color2, PPMImage *img) {
	if(iteration == 0) {
		drawMultipleCircles(xc, yc, radius, 0, 1, img, color1);
		return;
	}
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	drawMultipleCircles(xc, yc, radius, 0, 1, img, PURPLE);
	drawMultipleCircles(xc, yc + radius/2, radius/2, 0, 1, img, PURPLE);
	drawMultipleCircles(xc, yc - radius/2, radius/2, 0, 1, img, PURPLE);
	fillRegion(xc - radius/2, yc, color2, img->data[center_index + (xc-radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc - radius/2, color2, img->data[center_index + xc - img->width*(yc - radius/2) - 1], img);
	fillRegion(xc + radius/2, yc, color1, img->data[center_index + (xc+radius/2) - img->width*yc - 1], img);
	fillRegion(xc, yc + radius/2, color1, img->data[center_index + xc - img->width*(yc + radius/2) - 1], img);
	drawMultipleCircles(xc, yc + radius/2, (unsigned int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc + radius/2, color2, color1, img);
	drawMultipleCircles(xc, yc - radius/2, (unsigned int)(radius*smaller_radius_coefficient), 0, 1, img, PURPLE);
	fillRegion(xc, yc-radius/2, color1, color2, img);
	drawMultipleCircles(xc, yc - radius/2, radius/2, 0, 1, img, color2);
	drawYinYangFractal(xc, yc + radius/2, (unsigned int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);	//recursion
	drawYinYangFractal(xc, yc - radius/2, (unsigned int)(radius*smaller_radius_coefficient), smaller_radius_coefficient, iteration-1, color1, color2, img);
	drawMultipleCircles(xc, yc, radius, 0, 1, img, color1);
	drawMultipleCircles(xc, yc + radius/2, radius/2, 0, 1, img, color1);
}

// draws a circle fractal that spreads over the x axis on the given image
void drawCircleFractal_xAxis(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color) {
	drawMultipleCircles(xc, yc, radius, 0, 1, img, color);
	if(radius > 2) {
		drawCircleFractal_xAxis(xc + radius/2, yc, radius/2, img, color);		// recursion
		drawCircleFractal_xAxis(xc - radius/2, yc, radius/2, img, color);
	}
}

// draws a circle fractal that spreads over the xy plane on the given image
void drawCircleFractal_xyPlane(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color) {
	drawMultipleCircles(xc, yc, radius, 0, 1, img, color);
	if(radius > 8) {
		drawCircleFractal_xyPlane(xc + radius/2, yc, radius/2, img, color);		// recursion
		drawCircleFractal_xyPlane(xc - radius/2, yc, radius/2, img, color);
		drawCircleFractal_xyPlane(xc, yc + radius/2, radius/2, img, color);
		drawCircleFractal_xyPlane(xc, yc - radius/2, radius/2, img, color);
	}
}

// draws a tree fractal rooting from (x1,y1) on the given image
void drawTree(int x1, int y1, int main_angle, int branch_angle, unsigned int iteration, PPMImage *img, PPMPixel color) {
	if(iteration == 0)
		return;
	int x2 = x1 - (int)(cos(main_angle * M_PI / 180) * iteration * 10.0);
	int y2 = y1 - (int)(sin(main_angle * M_PI / 180) * iteration * 10.0);
	drawLine(x1, y1, x2, y2, img, color);
	drawTree(x2, y2, main_angle - branch_angle, branch_angle, iteration-1, img, color);		// recursion
	drawTree(x2, y2, main_angle + branch_angle, branch_angle, iteration-1, img, color);
}

// draws the Cantor set on the given image
void drawCantor(int x, int y, unsigned int length, PPMImage *img, PPMPixel color) {
	if(length >= 1) {
		drawLine(x, y, x + length, y, img, color);
		y -= 20;
		drawCantor(x, y, length/3, img, color);			// recursion
		drawCantor(x + 2*length/3, y, length/3, img, color);
	}
}

// draws the Barnsley fern on the given image, where the fractal gets more and more prominent as the "iteration" number increases
void drawBarnsleyFern(unsigned long long iteration, double size, PPMImage *img) {
	double x0 = 0, y0 = 0, x1, y1;
	int rand_num;
	srand((unsigned int)time(NULL));
	while(iteration--) {
		rand_num = rand()%100;
		if(rand_num == 0) {
			x1 = 0;
			y1 = 0.16*y0;
		}
		else if(rand_num <= 7) {
			x1 = -0.15*x0 + 0.28*y0;
			y1 = 0.26*x0 + 0.24*y0 + 0.44;
		}
		else if(rand_num <= 15) {
			x1 = 0.2*x0 - 0.26*y0;
			y1 = 0.23*x0 + 0.22*y0 + 1.6;
		}
		else {
			x1 = 0.85*x0 + 0.04*y0;
			y1 = -0.04*x0 + 0.85*y0 + 1.6;
		}
		img->data[img->width*(img->height-(int)round(size*y1)-1) + (int)round(size*x1 + img->width/2.0) ] = GREEN;
		x0 = x1, y0 = y1;
	}
}

#endif
