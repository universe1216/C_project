#ifndef _PPM_STRUCTS_H_
#define _PPM_STRUCTS_H_

typedef struct {
	unsigned char red,green,blue;
} PPMPixel;

typedef struct {
	unsigned int width,height;
	PPMPixel *data;
} PPMImage;

typedef struct {
	double x,y;
} Point;

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
void fillRegion(int x, int y, PPMPixel fill_color, PPMPixel region_color, PPMImage *img);
void drawTriangles(float x, float y, float h, PPMImage *img);
void drawTrianglesv2(float x, float y, float h, PPMImage *img);
void printSierpinski(float x, float y, float h, PPMImage *img);

#endif
