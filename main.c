#include "Header/imageManipulation.h"
#include "Header/fractals.h"

int main() {
	fprintf(stdout,"Process has started.\nPlease wait patiently...\n");


	PPMImage *img1 = readPPM("./Examples/Image Manipulation/Mix/west_1.ppm");
	rotateImage(img1, 2);
	sepiaFilter(img1);
	resizeImage(img1, 900, 700);
	writePPM("./Examples/Image Manipulation/Mix/west_1_rotated180_sepia_resized_900x700.ppm",img1);

	img1 = readPPM("./Examples/Image Manipulation/Mix/west_1.ppm");
	invertColor(img1);
	adjustBlur(img1, 4);
	adjustContrast(img1, 40);
	writePPM("./Examples/Image Manipulation/Mix/west_1_negative_blur4_contrast40.ppm",img1);

	img1 = readPPM("./Examples/Image Manipulation/Mix/west_1.ppm");
	adjustSaturation(img1, 60);
	adjustBrightness(img1, 20);
	adjustContrast(img1, 25);
	resizeImage(img1, 720, 480);
	writePPM("./Examples/Image Manipulation/Mix/west_1_saturated60_brightness20_contrast25_resized720x480.ppm",img1);

	deletePPMImage(img1);

	PPMImage *mix = createPPMImage(750, 750, BLACK);
	printKochCurve(-350, -350, 350, 350, 5, mix, RED);
	rotateImage(mix, 2);
	printMultipleCircles(0, 0, 300, 15, 20, mix, BLUE);
	writePPM("./Examples/Fractals/Mix/Koch curve and 15 circles.ppm",mix);

	mix = printMandelbrotSet(1800, 1800);
	printKochCurve(-850, -850, -250, -250, 5, mix, BLUE);
	printCircleFractal_xyPlane(-550, 550, 300, mix, PURPLE);
	drawTrianglesv2(550, -550, 400, mix, CYAN);
	printSierpinski(550, -550, 200, mix, GREEN);
	printTree(550, 300, -90, 10, 10, mix, YELLOW);
	writePPM("./Examples/Fractals/Mix/all_in_1.ppm",mix);

	deletePPMImage(mix);


	fprintf(stdout,"Image manipulation complete.\nProcess terminated successfully...");
	return 0;
}
