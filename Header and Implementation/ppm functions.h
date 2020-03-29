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

static PPMImage* readPPM(const char *filename) {
	char buff[16];
	PPMImage *img;
	FILE *fp;
	int rgb_comp_color;
	fp=fopen(filename,"rb");						//okumak için ppm dosyasý aç
	if(!fp) {
		fprintf(stderr,"Unable to open file '%s'.\n",filename);
		exit(1);
	}
	if(!fgets(buff,sizeof(buff),fp)) {				//resim dosyasýnýn formatýný oku
		perror(filename);
		exit(1);
	}
	if(buff[0]!='P' || buff[1]!='6') {				//resim dosyasýnýn formatýný kontrol et
		fprintf(stderr,"Invalid image format (must be 'P6').\n");
		exit(1);
	}
	img = (PPMImage*) malloc(sizeof(PPMImage));		//resim için dinamik bellek tahsisi yap
	if(!img) {
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(1);
	}
	char c=getc(fp);
	while(c=='#') {									//yorumlarý kontrol et
		while(getc(fp)!='\n');
		c=getc(fp);
	}
	ungetc(c,fp);
	if(fscanf(fp,"%d %d",&img->width,&img->height)!=2) {			//dosya boyutlarýný oku
		fprintf(stderr,"Invalid image size (error loading '%s').\n",filename);
		exit(1);
	}
	if(fscanf(fp,"%d",&rgb_comp_color)!=1) {						//RGB derinliðini oku
		fprintf(stderr,"Invalid RGB component (error loading '%s').\n",filename);
		exit(1);
	}
	if(rgb_comp_color!=RGB_COMPONENT_COLOR) {						//RGB derinliðini kontrol et
		fprintf(stderr,"'%s' does not have 8-bit components.\n",filename);
		exit(1);
	}
	while(fgetc(fp)!='\n');
	img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));	//pixel data için dinamik bellek tahsisi yap
	if(!img) {
		fprintf(stderr,"Unable to allocate memory.\n");
		exit(1);
	}
	if(fread(img->data, 3*img->width, img->height, fp) != img->height) {			//pixel datayý oku
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
	fp=fopen(filename,"wb");	//çýktý için dosya oluþtur
	if(!fp) {
		fprintf(stderr,"Unable to open file '%s'.\n",filename);
		exit(1);
	}
	fprintf(fp,"P6\n");										//resim formatý
	fprintf(fp,"# Created by %s\n",CREATOR);				//yorumlar
	fprintf(fp,"%d %d\n",img->width,img->height);			//resim boyutlarý
	fprintf(fp,"%d\n",RGB_COMPONENT_COLOR);					//RGB derinliði
	fwrite(img->data, 3*img->width, img->height, fp);		//pixel data
	fclose(fp);
}

static PPMImage* createPPMImage(unsigned int width, unsigned int height) {
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
	return img;
}

void invertColorPPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {			//girdinin her bir pixel'inin RGB deðerlerini karþýtýyla deðiþtir
			img->data[i].red = RGB_COMPONENT_COLOR - img->data[i].red;
			img->data[i].green = RGB_COMPONENT_COLOR - img->data[i].green;
			img->data[i].blue = RGB_COMPONENT_COLOR - img->data[i].blue;
		}
}

void grayscalePPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {			//girdinin her bir pixel'inin RGB deðerlerinin ortalamasýný hesaplayýp bu ortalamayý o pixel'in R,G ve B'sine ata
			double average = (img->data[i].red + img->data[i].green + img->data[i].blue) / 3.0;
			img->data[i].red = img->data[i].green = img->data[i].blue = average;
		}
}

void sepiaPPM(PPMImage *img) {
	unsigned int i;
	if(img)
		for(i=0;i<img->width*img->height;i++) {			//girdinin her bir pixel'inin R,G ve B deðerleri için ayrý aðýrlýklý ortalamalar hesaplayýp bu ortalamalarý o pixel'in sýrayla R,G ve B'sine ata
			double sepia_red = 0.393*img->data[i].red + 0.769*img->data[i].green + 0.189*img->data[i].blue;
			double sepia_green = 0.349*img->data[i].red + 0.686*img->data[i].green + 0.168*img->data[i].blue;
			double sepia_blue = 0.272*img->data[i].red + 0.534*img->data[i].green + 0.131*img->data[i].blue;
			img->data[i].red = sepia_red>255 ? 255 : sepia_red;
			img->data[i].green = sepia_green>255 ? 255 : sepia_green;
			img->data[i].blue = sepia_blue>255 ? 255 : sepia_blue;
		}
}

void rotate(PPMImage *img, short iteration) {
	unsigned short real_iteration = iteration>0 ? iteration%4 : (4 - (-iteration)%4) % 4, k;	//+: clockwise, -: counterclockwise
	if(img)
		for(k=0;k<real_iteration;k++) {
			PPMImage *buffer = createPPMImage(img->width,img->height);
			unsigned int i,j;
			for(i=0;i<img->height;i++)
				for(j=0;j<img->width;j++)
					buffer->data[img->height*(j+1) - i - 1] = img->data[img->width*i + j];		//img resmindeki pixel'leri buffer resmindeki pixel'lere kopyala
			memcpy(img->data, buffer->data, img->width * img->height * sizeof(PPMPixel));		//buffer pixel data'yý img pixel data'ya kopyala
			free(buffer);
			unsigned int temp = img->height;		//boyutlarý swap'la
			img->height = img->width;
			img->width = temp;
		}
}

static PPMImage* printRGBstrips(unsigned int width, unsigned int height, unsigned int iteration) {
	PPMImage *img = createPPMImage(width,3*height*iteration);
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
	PPMImage *img = createPPMImage(width,7*height*iteration);
	unsigned int i,j,k,counter=0;
	for(k=0;k<iteration;k++) {
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){255,0,0};			//red
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){255,127,0};		//orange
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){255,255,0};		//yellow
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){0,255,0};			//green
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){0,0,255};			//blue
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){75,0,130};		//indigo
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				img->data[counter++] = (PPMPixel){148,0,211};		//violet
	}
	return img;
}

static PPMImage* print8bitMandelbrotSet(unsigned int width, unsigned int height) {		//algoritma Uygulama 13'ten alýntý
	PPMImage *img = createPPMImage(width,height);
	int index=0,counter=0;
	float i,j,r,x,y;
	PPMPixel pixel_Array[16] = { {0,0,0}, {175,0,0}, {255,0,0}, {255,127,0},				//black, ROYGBIV
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
	PPMImage *img = createPPMImage(width,height);
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
			img->data[counter].red = img->data[counter].green = img->data[counter].blue = Iteration==IterationMax ? 0 : 255;	//(0,0,0) [black] : (255,255,255) [white]
			counter++;
		}
	}
	return img;
}

void drawCircle(int xc, int yc, int x, int y, PPMImage *img) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	img->data[center_index + xc + x - (yc + y)*img->width - 1] = (PPMPixel){255,0,0};		//çemberi oluþturan 8 ayrý bölmedeki bütün pixel'lerin rengi kýrmýzý
	img->data[center_index + xc - x - (yc + y)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc + x - (yc - y)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc - x - (yc - y)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc + y - (yc + x)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc - y - (yc + x)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc + y - (yc - x)*img->width - 1] = (PPMPixel){255,0,0};
	img->data[center_index + xc - y - (yc - x)*img->width - 1] = (PPMPixel){255,0,0};
}

static PPMImage* printCircles(unsigned int width, unsigned int height, int xc, int yc, unsigned int r, unsigned int gap, unsigned int frequency) {
	PPMImage *img = createPPMImage(width,height);
	while(frequency--) {
		int x=0, y=r;
		int d=3-2*r;
		drawCircle(xc,yc,x,y,img);
		while(y>=x) {			//Bresenham'ýn çember çizme algoritmasý
			x++;
			if(d>0) {
				y--;
				d+=4*(x-y)+10;
			}
			else
				d+=4*x+6;
			drawCircle(xc,yc,x,y,img);
		}
		r-=gap;
	}
	unsigned int i;
	for(i=0;i<width*height;i++)
		if(!(img->data[i].red==255 && img->data[i].green==0 && img->data[i].blue==0))
			img->data[i] = (PPMPixel){0,0,0};		//arka plan rengi siyah
	return img;
}

static PPMImage* printRoughHeart(unsigned int size) {				//kulakçýk büyüklüðünü (auricle size) parametre olarak alýr
	PPMImage *img = size%4==1 || size%4==2 ? createPPMImage(2*size-1,floor(5*size/4.0)+1) : createPPMImage(2*size,floor(5*size/4.0)+2);
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

void drawLine(int x1, int y1, int x2, int y2, PPMImage *img) {
	unsigned int center_index = (img->width*img->height)%4==0 ? img->width*(img->height-1)/2: img->width*img->height/2;
	int dx=x2-x1, dy=y2-y1;
	unsigned int steps = abs(dx)>abs(dy) ? abs(dx) : abs(dy);
	float Xinc = dx/(float)steps, Yinc = dy/(float)steps, x=x1, y=y1;
	unsigned int i;
	for(i=0; i<=steps; i++, x+=Xinc, y+=Yinc)
		img->data[center_index + (int)x - img->width*(int)y - 1] = (PPMPixel){255,0,0};
	for(i=0;i<img->width*img->height;i++)
		if(!(img->data[i].red==255 && img->data[i].green==0 && img->data[i].blue==0))
			img->data[i] = (PPMPixel){0,0,0};
}

void drawKochCurve(Point p1, Point p2, unsigned int iteration, PPMImage *img) {
	Point p3,p4,p5;
	long double theta = M_PI/3;
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
		drawLine(p1.x,p1.y,p3.x,p3.y,img);
		drawLine(p3.x,p3.y,p5.x,p5.y,img);
		drawLine(p5.x,p5.y,p4.x,p4.y,img);
		drawLine(p4.x,p4.y,p2.x,p2.y,img);
	}
}

void printKochCurve(int x1, int y1, int x2, int y2, unsigned int iteration, PPMImage *img) {
	Point p1 = (Point){x1,y1};
	Point p2 = (Point){x2,y2};
	drawKochCurve(p1,p2,iteration,img);
}

#endif
