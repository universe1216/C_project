#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"
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

static PPMImage* readPPM(const char *filename);
void writePPM(const char *filename, PPMImage *img);
static PPMImage* createPPMImage(unsigned int width, unsigned int height, PPMPixel color);
void invertColorPPM(PPMImage *img);
void grayscalePPM(PPMImage *img);
void sepiaPPM(PPMImage *img);
void rotate(PPMImage *img, short iteration);
static PPMImage* printRGBstrips(unsigned int width, unsigned int height, unsigned int iteration);
static PPMImage* printRainbow(unsigned int width, unsigned int height, unsigned int iteration);
static PPMImage* print8bitMandelbrotSet(unsigned int width, unsigned int height);
static PPMImage* printEpicMandelbrotSet(unsigned int width, unsigned int height);
void drawCircle(int xc, int yc, int x, int y, PPMImage *img, PPMPixel boundary_color);
void printCircles(int xc, int yc, unsigned int r, unsigned int gap, unsigned int frequency, PPMImage *img, PPMPixel boundary_color);
static PPMImage* printRoughHeart(unsigned int size);
void drawLine(int x1, int y1, int x2, int y2, PPMImage *img, PPMPixel line_color);
void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img);
void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img);
int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2);
void fillRegion(int x, int y, PPMPixel fill_color, PPMPixel region_color, PPMImage *img);
void drawTriangles(float x, float y, float h, PPMImage *img);
void drawTrianglesv2(float x, float y, float h, PPMImage *img);
void printSierpinski(float x, float y, float h, PPMImage *img);
static PPMImage* printJulia(int width, int height, int zoom);
double* convertHSVtoRGB(double h, double s, double v);

#endif
