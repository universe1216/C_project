#ifndef _DECLARATIONS_H_
#define _DECLARATIONS_H_

#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define CREATOR "DasDasAras"
#define RGB_COMPONENT_COLOR 255		//each component of the RGB color model defines the intensity of the color between 0-255

// colors
const PPMPixel RED			=	(PPMPixel){255,0,0};
const PPMPixel GREEN		=	(PPMPixel){0,255,0};
const PPMPixel BLUE			=	(PPMPixel){0,0,255};
const PPMPixel MAROON		=	(PPMPixel){127,0,0};
const PPMPixel ORANGE		=	(PPMPixel){255,127,0};
const PPMPixel YELLOW		=	(PPMPixel){255,255,0};
const PPMPixel CHARTREUSE	=	(PPMPixel){127,255,0};
const PPMPixel SPRING_GREEN	=	(PPMPixel){0,255,127};
const PPMPixel AZURE		=	(PPMPixel){0,127,255};
const PPMPixel CYAN			=	(PPMPixel){0,255,255};
const PPMPixel ROSE			=	(PPMPixel){255,0,127};
const PPMPixel MAGENTA		=	(PPMPixel){255,0,255};
const PPMPixel PURPLE		=	(PPMPixel){127,0,127};
const PPMPixel INDIGO		=	(PPMPixel){75,0,130};
const PPMPixel VIOLET		=	(PPMPixel){127,0,255};
const PPMPixel DARK_VIOLET	=	(PPMPixel){148,0,211};
const PPMPixel BROWN		=	(PPMPixel){150,75,0};
const PPMPixel BLACK		=	(PPMPixel){0,0,0};
const PPMPixel GRAY			=	(PPMPixel){127,127,127};
const PPMPixel WHITE		=	(PPMPixel){255,255,255};

// function prototypes
PPMImage* readPPM(const char *filename);
void writePPM(const char *filename, PPMImage *img);
PPMImage* createPPMImage(unsigned int image_width, unsigned int image_height, PPMPixel background_color);
void deletePPMImage(PPMImage *img);
void copyPPMImage(PPMImage *image1, PPMImage *image2);
void putPixel(int x, int y, PPMImage *img, PPMPixel color);
unsigned int arePixelsEqual(PPMPixel pixel1, PPMPixel pixel2);
double truncateRGB(double rgb);
double* convertHSVtoRGB(double h, double s, double v);
double* convertHSLtoRGB(double h, double s, double l);
double* convertRGBtoHSV(double r, double g, double b);
double* convertRGBtoHSL(double r, double g, double b);
void invertColor(PPMImage *img);
void meanGrayscaleFilter(PPMImage *img);
void weightedGrayscaleFilter(PPMImage *img);
void sepiaFilter(PPMImage *img);
void pixelate(PPMImage *img, unsigned int pixel_size);
void rotateImage(PPMImage *img, short iteration);
void resizeImage(PPMImage *img, unsigned int new_image_width, unsigned int new_image_height);
void adjustSaturation(PPMImage *img, double saturation_percentage);
void adjustBlur(PPMImage *img, double sigma);
void adjustContrast(PPMImage *img, double contrast_percentage);
void adjustBrightness(PPMImage *img, double brightness_percentage);
void adjustHue(PPMImage *img, int angle);
void gammaCorrection(PPMImage *img, double gamma);
void drawCircle(int xc, int yc, int x, int y, PPMImage *img, PPMPixel boundary_color);
void printMultipleCircles(int xc, int yc, unsigned int radius, unsigned int gap, unsigned int frequency, PPMImage *img, PPMPixel color);
void drawLine(int x1, int y1, int x2, int y2, PPMImage *img, PPMPixel color);
void fillRegion(int x, int y, PPMPixel fill_color, PPMPixel region_color, PPMImage *img);
PPMImage* printMandelbrotSet(unsigned int image_width, unsigned int image_height);
void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img, PPMPixel color);
void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img, PPMPixel color);
void drawTriangles(double x, double y, double h, PPMImage *img, PPMPixel color);
void drawTrianglesv2(double x, double y, double h, PPMImage *img, PPMPixel color);
void printSierpinski(double x, double y, double h, PPMImage *img, PPMPixel color);
PPMImage* printJulia(int image_width, int image_height, int zoom);
void printYinYangFractal(int xc, int yc, unsigned int radius, double smaller_radius_coefficient, unsigned int iteration, PPMPixel color1, PPMPixel color2, PPMImage *img);
void printCircleFractal_xAxis(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color);
void printCircleFractal_xyPlane(int xc, int yc, unsigned int radius, PPMImage *img, PPMPixel color);
void printTree(int x1, int y1, int main_angle, int branch_angle, unsigned int iteration, PPMImage *img, PPMPixel color);
void printCantor(int x, int y, unsigned int width, PPMImage *img, PPMPixel color);
void printBarnsleyFern(unsigned long long iteration, double size, PPMImage *img);

#endif
