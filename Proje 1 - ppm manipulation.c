#include "Header and Implementation/ppm functions.h"

/* Okunan PPM dosyalarýný manipüle ederek ya da desen ve þekil çizme algoritmalarý kullanarak yeni PPM dosyalarý oluþturan Ýngilizce program */

int main() {
	PPMImage *img1=readPPM("./Examples/Filters/Negative/west_1.ppm");
	invertColorPPM(img1);
	writePPM("./Examples/Filters/Negative/west_1_negative.ppm",img1);

	img1=readPPM("./Examples/Filters/Grayscale/west_1.ppm");
	grayscalePPM(img1);
	writePPM("./Examples/Filters/Grayscale/west_1_grayscale.ppm",img1);

	img1=readPPM("./Examples/Filters/Sepia/west_1.ppm");
	sepiaPPM(img1);
	writePPM("./Examples/Filters/Sepia/west_1_sepia.ppm",img1);

	img1=readPPM("./Examples/Filters/Rotate/west_1.ppm");
	rotate(img1,1);
	writePPM("./Examples/Filters/Rotate/west_1_rotated90.ppm",img1);

	img1=readPPM("./Examples/Filters/Rotate/west_1.ppm");
	rotate(img1,2);
	writePPM("./Examples/Filters/Rotate/west_1_rotated180.ppm",img1);

	img1=readPPM("./Examples/Filters/Rotate/west_1.ppm");
	rotate(img1,-1);
	writePPM("./Examples/Filters/Rotate/west_1_rotated270.ppm",img1);

	img1=readPPM("./Examples/Filters/Rotate/west_1.ppm");
	rotate(img1,4);
	writePPM("./Examples/Filters/Rotate/west_1_rotated360.ppm",img1);

	free(img1);



	PPMImage *rgb_strips;
	rgb_strips=printRGBstrips(256,1,1);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_256x1_1.ppm",rgb_strips);

	rgb_strips=printRGBstrips(256,3,3);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_256x3_3.ppm",rgb_strips);

	rgb_strips=printRGBstrips(128,6,1);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_128x6_1.ppm",rgb_strips);

	rgb_strips=printRGBstrips(512,12,4);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_512x12_4.ppm",rgb_strips);

	rgb_strips=printRGBstrips(1024,9,3);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_1024x9_3.ppm",rgb_strips);

	free(rgb_strips);


	PPMImage *rainbow=printRainbow(500,9,1);
	writePPM("./Examples/Patterns/Rainbow/just a typical rainbow.ppm",rainbow);

	rainbow=printRainbow(700,9,2);
	writePPM("./Examples/Patterns/Rainbow/double rainbow yaaay.ppm",rainbow);

	rainbow=printRainbow(1000,9,3);
	rotate(rainbow,-1);
	writePPM("./Examples/Patterns/Rainbow/vertical triple rainbow xd.ppm",rainbow);

	free(rainbow);


	PPMImage *Mandelbrot=print8bitMandelbrotSet(84,31);
	writePPM("./Examples/Fractals/Mandelbrot Set/8_bit_Mandelbrot_set_84x31.ppm",Mandelbrot);

	Mandelbrot=printEpicMandelbrotSet(800,800);
	writePPM("./Examples/Fractals/Mandelbrot Set/Real Mandelbrot set 800x800.ppm",Mandelbrot);

	free(Mandelbrot);


	PPMImage *circle=printCircles(500,500,0,0,100,0,1);
	writePPM("./Examples/Patterns/Circle/circle_500x500_100.ppm",circle);

	circle=printCircles(250,250,-74,74,50,0,1);
	writePPM("./Examples/Patterns/Circle/circle_250x250_50.ppm",circle);

	circle=printCircles(500,500,0,0,100,5,20);
	writePPM("./Examples/Patterns/Circle/20xcircles_500x500_100.ppm",circle);

	circle=printCircles(500,500,0,0,200,2,100);
	writePPM("./Examples/Patterns/Circle/100xcircles_500x500_200.ppm",circle);

	free(circle);


	PPMImage *heart=printRoughHeart(32);
	writePPM("./Examples/Patterns/Heart/sharp_heart_S.ppm",heart);

	heart=printRoughHeart(81);
	writePPM("./Examples/Patterns/Heart/sharp_heart_M.ppm",heart);

	heart=printRoughHeart(150);
	writePPM("./Examples/Patterns/Heart/sharp_heart_L.ppm",heart);

	heart=printRoughHeart(255);
	writePPM("./Examples/Patterns/Heart/sharp_heart_XL.ppm",heart);

	free(heart);


	PPMImage *line=createPPMImage(250,250);
	drawLine(0,0,50,100,line);
	writePPM("./Examples/Patterns/Line/line_1.ppm",line);

	line=createPPMImage(500,500);
	drawLine(0,0,100,20,line);
	drawLine(0,0,100,-100,line);
	drawLine(0,-100,-100,0,line);
	writePPM("./Examples/Patterns/Line/4xlines_1.ppm",line);

	free(line);


	PPMImage *Koch=createPPMImage(1000,1000);
	printKochCurve(-450,-450,450,450,3,Koch);
	writePPM("./Examples/Fractals/Koch Curve/Koch_curve.ppm",Koch);

	Koch=createPPMImage(1000,1000);
	printKochCurve(-450,400,-350,450,3,Koch);
	printKochCurve(-450,0,0,450,4,Koch);
	printKochCurve(-450,-450,450,450,5,Koch);
	writePPM("./Examples/Fractals/Koch Curve/3xKoch_curve.ppm",Koch);

	free(Koch);


	fprintf(stdout,"Image manipulation complete.\nProcess terminated successfully...");
	exit(0);
}
