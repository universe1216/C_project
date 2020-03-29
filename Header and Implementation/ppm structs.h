#ifndef _PPM_STRUCTS_H_
#define _PPM_STRUCTS_H_

typedef struct {
	unsigned char red,green,blue;		//pixel'lerin RGB deðerleri 0-255 arasýnda deðiþir ve PPM dosyalarýnda bu deðerlere ASCII kodu denk olan karakterler ile ifade edilir
} PPMPixel;								//RGB deðerleri karakter veya 0-255 arasýnda sayýlar olmak üzere pixel data:
										//R(1-1)G(1-1)B(1-1) R(1-2)G(1-2)B(1-2) ... R(1-w)G(1-w)B(1-w)
typedef struct {						//R(2-1)G(2-1)B(2-1) R(2-2)G(2-2)B(2-2) ... R(2-w)G(2-w)B(2-w)
	unsigned int width,height;			//...
	PPMPixel *data;						//R(h-1)G(h-1)B(h-1) R(h-2)G(h-2)B(h-2) ... R(h-w)G(h-w)B(h-w)		biçiminde PPM dosyasýnda saklanýr ve bu pixel'ler bütünü bir resmi oluþturur
} PPMImage;

typedef struct {
	double x,y;
} Point;

static PPMImage* readPPM(const char *filename);					//fonksiyon prototipleri
void writePPM(const char *filename, PPMImage *img);
static PPMImage* createPPMImage(unsigned int width, unsigned int height);
void invertColorPPM(PPMImage *img);
void grayscalePPM(PPMImage *img);
void sepiaPPM(PPMImage *img);
void rotate(PPMImage *img, short iteration);
static PPMImage* printRGBstrips(unsigned int width, unsigned int height, unsigned int iteration);
static PPMImage* printRainbow(unsigned int width, unsigned int height, unsigned int iteration);
static PPMImage* print8bitMandelbrotSet(unsigned int width, unsigned int height);
static PPMImage* printEpicMandelbrotSet(unsigned int width, unsigned int height);
void drawCircle(int xc, int yc, int x, int y, PPMImage *img);
static PPMImage* printCircles(unsigned int width, unsigned int height, int xc, int yc, unsigned int r, unsigned int gap, unsigned int frequency);
static PPMImage* printRoughHeart(unsigned int size);
void drawLine(int x1, int y1, int x2, int y2, PPMImage *img);
void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img);
void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img);

#endif
