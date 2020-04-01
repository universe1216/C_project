#include "Header and Implementation/filters.h"
#include "Header and Implementation/patterns.h"
#include "Header and Implementation/fractals.h"

int main() {
	fprintf(stdout,"Process has started.\nPlease wait patiently...\n");


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
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_256x1_i1.ppm",rgb_strips);

	rgb_strips=printRGBstrips(256,3,3);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_256x3_i3.ppm",rgb_strips);

	rgb_strips=printRGBstrips(128,6,1);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_128x6_i1.ppm",rgb_strips);

	rgb_strips=printRGBstrips(512,12,4);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_512x12_i4.ppm",rgb_strips);

	rgb_strips=printRGBstrips(1024,9,3);
	writePPM("./Examples/Patterns/RGB Strips/rgb_strips_1024x9_i3.ppm",rgb_strips);

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
	writePPM("./Examples/Fractals/Mandelbrot Set/Real_Mandelbrot_set_800x800.ppm",Mandelbrot);

	free(Mandelbrot);



	PPMImage *circle=createPPMImage(500,500,BLACK);
	printMultipleCircles(0,0,100,0,1,circle,RED);
	writePPM("./Examples/Patterns/Circle/circle_500x500_r100.ppm",circle);

	circle=createPPMImage(250,250,BLACK);
	printMultipleCircles(-74,74,50,0,1,circle,RED);
	writePPM("./Examples/Patterns/Circle/circle_250x250_r50.ppm",circle);

	circle=createPPMImage(500,500,BLACK);
	printMultipleCircles(0,0,100,5,20,circle,RED);
	writePPM("./Examples/Patterns/Circle/20xcircles_500x500_r100.ppm",circle);

	circle=createPPMImage(500,500,BLACK);
	printMultipleCircles(0,0,200,2,100,circle,RED);
	writePPM("./Examples/Patterns/Circle/100xcircles_500x500_r200.ppm",circle);

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



	PPMImage *line=createPPMImage(250,250,WHITE);
	drawLine(0,0,50,100,line,RED);
	writePPM("./Examples/Patterns/Line/line_1.ppm",line);

	line=createPPMImage(500,500,BLACK);
	drawLine(0,0,100,20,line,RED);
	drawLine(0,0,100,-100,line,RED);
	drawLine(0,-100,-100,0,line,RED);
	writePPM("./Examples/Patterns/Line/3xlines_1.ppm",line);

	line=createPPMImage(500,500,BLACK);
	drawLine(200,200,100,100,line,RED);
	drawLine(0,100,-200,100,line,BLUE);
	drawLine(0,-249,0,249,line,GREEN);
	drawLine(0,-100,-100,0,line,PURPLE);
	drawLine(-150,-100,200,150,line,YELLOW);
	writePPM("./Examples/Patterns/Line/5xlines_1.ppm",line);

	free(line);



	PPMImage *Koch=createPPMImage(1000,1000,BLACK);
	printKochCurve(-450,-450,450,450,3,Koch,RED);
	writePPM("./Examples/Fractals/Koch Curve/Koch_curve.ppm",Koch);

	Koch=createPPMImage(1000,1000,BLACK);
	printKochCurve(-450,400,-350,450,3,Koch,RED);
	printKochCurve(-450,0,0,450,4,Koch,GREEN);
	printKochCurve(-450,-450,450,450,7,Koch,BLUE);
	writePPM("./Examples/Fractals/Koch Curve/3xKoch_curve.ppm",Koch);

	Koch=createPPMImage(1000,1000,BLACK);
	printKochCurve(-450,-450,450,450,5,Koch,BLUE);
	rotate(Koch,2);
	printKochCurve(-450,-450,450,450,5,Koch,BLUE);
	writePPM("./Examples/Fractals/Koch Curve/symmetrical_Koch_curve.ppm",Koch);

	free(Koch);



	PPMImage *filledShape=createPPMImage(300,300,WHITE);
	printMultipleCircles(0,0,100,0,1,filledShape,RED);
	fillRegion(0,0,RED,WHITE,filledShape);
	writePPM("./Examples/Patterns/Filled Shapes/red_disk.ppm",filledShape);

	filledShape=createPPMImage(300,300,WHITE);
	drawLine(-100,-100,0,75,filledShape,RED);
	drawLine(100,-100,0,75,filledShape,GREEN);
	drawLine(-100,-100,100,-100,filledShape,YELLOW);
	fillRegion(0,0,BLUE,WHITE,filledShape);
	writePPM("./Examples/Patterns/Filled Shapes/blue_triangle.ppm",filledShape);

	filledShape=createPPMImage(310,310,WHITE);
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
	writePPM("./Examples/Patterns/Filled Shapes/yellow_star.ppm",filledShape);

	free(filledShape);



	PPMImage *Sierpinski=createPPMImage(1200,1200,BLACK);
	drawTrianglesv2(0,100,900,Sierpinski,RED);
	printSierpinski(0,100,450,Sierpinski,RED);
	writePPM("./Examples/Fractals/Sierpinski Triangle/Sierpinski_triangle.ppm",Sierpinski);

	free(Sierpinski);



	PPMImage *Julia=printJulia(1920,1080,1);
	writePPM("./Examples/Fractals/Julia Set/Julia_set_z1.ppm",Julia);

	Julia=printJulia(1920,1080,5);
	writePPM("./Examples/Fractals/Julia Set/Julia_set_z5.ppm",Julia);

	Julia=printJulia(1920,1080,10);
	writePPM("./Examples/Fractals/Julia Set/Julia_set_z10.ppm",Julia);

	Julia=printJulia(1920,1080,20);
	writePPM("./Examples/Fractals/Julia Set/Julia_set_z20.ppm",Julia);

	free(Julia);



	PPMImage *yinyang=createPPMImage(500,500,BLACK);
	printYinYangFractal(0,0,200,0.20,1,RED,WHITE,yinyang);
	writePPM("./Examples/Fractals/Yin Yang/yin_yang_r200_i1.ppm",yinyang);

	yinyang=createPPMImage(600,600,GREEN);
	printYinYangFractal(0,0,250,0.33,3,BLACK,WHITE,yinyang);
	writePPM("./Examples/Fractals/Yin Yang/yin_yang_r250_i3.ppm",yinyang);

	yinyang=createPPMImage(1200,1200,YELLOW);
	printYinYangFractal(0,0,500,0.33,4,BLACK,WHITE,yinyang);
	writePPM("./Examples/Fractals/Yin Yang/yin_yang_r500_i4.ppm",yinyang);

	yinyang=createPPMImage(1300,1300,RED);
	printYinYangFractal(0,0,560,0.40,5,BLACK,WHITE,yinyang);
	writePPM("./Examples/Fractals/Yin Yang/yin_yang_r560_i5.ppm",yinyang);

	free(yinyang);



	PPMImage *circleFractal=createPPMImage(1080,720,WHITE);
	printCircleFractal_xAxis(0,0,300,circleFractal,BLACK);
	writePPM("./Examples/Fractals/Circle/circle_fractal_r300_x_axis.ppm",circleFractal);

	circleFractal=createPPMImage(1920,1080,BLACK);
	printCircleFractal_xyPlane(0,0,500,circleFractal,RED);
	writePPM("./Examples/Fractals/Circle/circle_fractal_r500_xy_plane.ppm",circleFractal);

	free(circleFractal);



	PPMImage *tree=tree=createPPMImage(150,150,WHITE);
	printTree(0,-60,-90,20,4,tree,BLACK);
	writePPM("./Examples/Fractals/Tree/tree_d20_i4.ppm",tree);

	tree=createPPMImage(600,600,BLACK);
	printTree(0,-290,-90,20,9,tree,WHITE);
	writePPM("./Examples/Fractals/Tree/tree_d20_i9.ppm",tree);

	tree=createPPMImage(1300,1000,BLACK);
	printTree(0,-490,-90,20,13,tree,YELLOW);
	writePPM("./Examples/Fractals/Tree/tree_d20_i13.ppm",tree);

	tree=createPPMImage(200,150,BLACK);
	printTree(0,-60,-90,90,6,tree,YELLOW);
	writePPM("./Examples/Fractals/Tree/tree_d90_i6.ppm",tree);

	tree=createPPMImage(300,300,BLACK);
	printTree(50,-50,-60,-135,9,tree,YELLOW);
	writePPM("./Examples/Fractals/Tree/kite.ppm",tree);

	free(tree);



	PPMImage *Cantor=createPPMImage(600,200,BLACK);
	printCantor(-250,50,500,Cantor,RED);
	writePPM("./Examples/Fractals/Cantor Set/Cantor_set.ppm",Cantor);

	free(Cantor);



	PPMImage *mix=createPPMImage(750,750,BLACK);
	printKochCurve(-350,-350,350,350,5,mix,RED);
	rotate(mix,2);
	printMultipleCircles(0,0,300,15,20,mix,BLUE);
	writePPM("./Examples/Fractals/Mix/Koch curve and 15 circles.ppm",mix);

	mix=printEpicMandelbrotSet(1800,1800);
	printKochCurve(-850,-850,-250,-250,5,mix,BLUE);
	printMultipleCircles(-550,550,290,8,50,mix,PURPLE);
	drawTrianglesv2(550,-550,400,mix,CYAN);
	printSierpinski(550,-550,200,mix,GREEN);
	printTree(550,300,-90,10,10,mix,YELLOW);
	writePPM("./Examples/Fractals/Mix/all_in_1.ppm",mix);

	free(mix);



	fprintf(stdout,"Image manipulation complete.\nProcess terminated successfully...");
	exit(0);
}
