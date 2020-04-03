#include "Header/imageManipulation.h"
#include "Header/fractals.h"

int main() {
	fprintf(stdout,"Process has started.\nPlease wait patiently...\n");


	/* image manipulation */


	PPMImage *img1=readPPM("./Examples/Image Manipulation/Negative/west_1.ppm");
	invertColor(img1);
	writePPM("./Examples/Image Manipulation/Negative/west_1_negative.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Grayscale/west_1.ppm");
	grayscaleFilter(img1);
	writePPM("./Examples/Image Manipulation/Grayscale/west_1_grayscale.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Sepia/west_1.ppm");
	sepiaFilter(img1);
	writePPM("./Examples/Image Manipulation/Sepia/west_1_sepia.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Rotate/west_1.ppm");
	rotateImage(img1,1);
	writePPM("./Examples/Image Manipulation/Rotate/west_1_rotated90.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Resize/west_1.ppm");
	resizeImage(img1,1280,760);
	writePPM("./Examples/Image Manipulation/Resize/west_1_resized_1280x760.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Saturate/west_1.ppm");
	adjustSaturation(img1,50);
	writePPM("./Examples/Image Manipulation/Saturate/west_1_saturated_50.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Blur/west_1.ppm");
	adjustBlur(img1,3);
	writePPM("./Examples/Image Manipulation/Blur/west_1_blurred_3.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Contrast/west_1.ppm");
	adjustContrast(img1,50);
	writePPM("./Examples/Image Manipulation/Contrast/west_1_contrast_50.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Brightness/west_1.ppm");
	adjustBrightness(img1,50);
	writePPM("./Examples/Image Manipulation/Brightness/west_1_brightness_50.ppm",img1);



	img1=readPPM("./Examples/Image Manipulation/Mix/west_1.ppm");
	rotateImage(img1,2);
	sepiaFilter(img1);
	resizeImage(img1,900,700);
	writePPM("./Examples/Image Manipulation/Mix/west_1_rotated180_sepia_resized_900x700.ppm",img1);

	free(img1);


	/* shapes and patterns */


	PPMImage *rgb_strips=printRGBstrips(512,12,4);
	writePPM("./Examples/Shapes and Patterns/RGB Strips/rgb_strips_512x12_i4.ppm",rgb_strips);

	free(rgb_strips);



	PPMImage *rainbow=printRainbow(1000,9,3);
	rotateImage(rainbow,-1);
	writePPM("./Examples/Shapes and Patterns/Rainbow/vertical triple rainbow.ppm",rainbow);

	free(rainbow);



	PPMImage *circle=createPPMImage(500,500,BLACK);
	printMultipleCircles(0,0,100,5,20,circle,RED);
	writePPM("./Examples/Shapes and Patterns/Circle/20xcircles_500x500_r100.ppm",circle);

	free(circle);



	PPMImage *heart=printRoughHeart(81);
	writePPM("./Examples/Shapes and Patterns/Heart/sharp_heart_M.ppm",heart);

	free(heart);



	PPMImage *line=createPPMImage(500,500,BLACK);
	drawLine(200,200,100,100,line,RED);
	drawLine(0,100,-200,100,line,BLUE);
	drawLine(0,-249,0,249,line,GREEN);
	drawLine(0,-100,-100,0,line,PURPLE);
	drawLine(-150,-100,200,150,line,YELLOW);
	writePPM("./Examples/Shapes and Patterns/Line/5xlines_1.ppm",line);

	free(line);



	PPMImage *filledShape=createPPMImage(310,310,WHITE);
	drawLine(0,144,-36,60,filledShape,YELLOW);
	drawLine(-36,60,-120,60,filledShape,YELLOW);
	drawLine(-120,60,-48,0,filledShape,YELLOW);
	drawLine(-48,0,-84,-108,filledShape,YELLOW);
	drawLine(-84,-108,0,-48,filledShape,YELLOW);
	drawLine(0,-48,84,-108,filledShape,YELLOW);
	drawLine(84,-108,48,0,filledShape,YELLOW);
	drawLine(48,0,120,60,filledShape,YELLOW);
	drawLine(120,60,36,60,filledShape,YELLOW);
	drawLine(36,60,0,144,filledShape,YELLOW);
	fillRegion(0,0,YELLOW,WHITE,filledShape);
	writePPM("./Examples/Shapes and Patterns/Filled Shapes/yellow_star.ppm",filledShape);

	free(filledShape);


	/* fractals */


	PPMImage *Mandelbrot=printEpicMandelbrotSet(800,800);
	writePPM("./Examples/Fractals/Mandelbrot Set/Real_Mandelbrot_set_800x800.ppm",Mandelbrot);

	free(Mandelbrot);



	PPMImage *Koch=createPPMImage(1000,1000,BLACK);
	printKochCurve(-450,400,-350,450,3,Koch,RED);
	printKochCurve(-450,0,0,450,4,Koch,GREEN);
	printKochCurve(-450,-450,450,450,7,Koch,BLUE);
	writePPM("./Examples/Fractals/Koch Curve/3xKoch_curve.ppm",Koch);

	free(Koch);



	PPMImage *Sierpinski=createPPMImage(1200,1200,BLACK);
	drawTrianglesv2(0,100,900,Sierpinski,RED);
	printSierpinski(0,100,450,Sierpinski,RED);
	writePPM("./Examples/Fractals/Sierpinski Triangle/Sierpinski_triangle.ppm",Sierpinski);

	free(Sierpinski);



	PPMImage *Julia=printJulia(1920,1080,1);
	writePPM("./Examples/Fractals/Julia Set/Julia_set_z1.ppm",Julia);

	free(Julia);



	PPMImage *yinyang=createPPMImage(1300,1300,RED);
	printYinYangFractal(0,0,560,0.40,5,BLACK,WHITE,yinyang);
	writePPM("./Examples/Fractals/Yin Yang/yin_yang_r560_i5.ppm",yinyang);

	free(yinyang);



	PPMImage *circleFractal=createPPMImage(1920,1080,BLACK);
	printCircleFractal_xyPlane(0,0,500,circleFractal,RED);
	writePPM("./Examples/Fractals/Circle/circle_fractal_r500_xy_plane.ppm",circleFractal);

	free(circleFractal);



	PPMImage *tree=createPPMImage(1300,1000,BLACK);
	printTree(0,-490,-90,20,13,tree,YELLOW);
	writePPM("./Examples/Fractals/Tree/tree_d20_i13.ppm",tree);

	free(tree);



	PPMImage *Cantor=createPPMImage(600,200,BLACK);
	printCantor(-250,50,500,Cantor,RED);
	writePPM("./Examples/Fractals/Cantor Set/Cantor_set.ppm",Cantor);

	free(Cantor);



	PPMImage *fern=createPPMImage(800,800,RED);
	printBarnsleyFern(1000000,75,fern);
	writePPM("./Examples/Fractals/Barnsley Fern/fern_i1000000.ppm",fern);

	free(fern);



	PPMImage *mix=printEpicMandelbrotSet(1800,1800);
	printKochCurve(-850,-850,-250,-250,5,mix,BLUE);
	printCircleFractal_xyPlane(-550,550,300,mix,PURPLE);
	drawTrianglesv2(550,-550,400,mix,CYAN);
	printSierpinski(550,-550,200,mix,GREEN);
	printTree(550,300,-90,10,10,mix,YELLOW);
	writePPM("./Examples/Fractals/Mix/all_in_1.ppm",mix);

	free(mix);



	fprintf(stdout,"Image manipulation complete.\nProcess terminated successfully...");
	exit(0);
}
