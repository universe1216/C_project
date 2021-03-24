#include "Header/image_manipulation.h"
#include "Header/fractals.h"

int main() {
	fprintf(stdout,"Process has started.\nPlease wait patiently...\n");


	PPMImage *img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	invertColor(img);
	writePPM("./Examples/Image Manipulation/west_1_negative.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	weightedGrayscaleFilter(img);
	writePPM("./Examples/Image Manipulation/west_1_grayscale.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	sepiaFilter(img);
	writePPM("./Examples/Image Manipulation/west_1_sepia.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustSaturation(img, 50);
	writePPM("./Examples/Image Manipulation/west_1_saturated_50.ppm",img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	resizeImage(img, 1280, 760);
	writePPM("./Examples/Image Manipulation/west_1_resized_1280x760.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustHue(img, 135);
	writePPM("./Examples/Image Manipulation/west_1_hue_135.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustBrightness(img, 50);
	writePPM("./Examples/Image Manipulation/west_1_brightness_50.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustContrast(img, 50);
	writePPM("./Examples/Image Manipulation/west_1_contrast_50.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustBlur(img, 3);
	writePPM("./Examples/Image Manipulation/west_1_blurred_3.ppm", img);
	deletePPMImage(img);



	img = createPPMImage(750, 750, BLACK);
	drawKochCurve(-350, -350, 350, 350, 5, img, RED);
	rotateImage(img, 2);
	drawMultipleCircles(0, 0, 300, 15, 20, img, BLUE);
	writePPM("./Examples/Fractals/Koch_curve_and_15_circles.ppm", img);
	deletePPMImage(img);

	img = drawMandelbrotSet(1800, 1800);
	drawKochCurve(-850, -850, -250, -250, 5, img, BLUE);
	drawCircleFractal_xyPlane(-550, 550, 300, img, PURPLE);
	drawSierpinskiUtil2(550, -550, 400, img, CYAN);
	drawSierpinski(550, -550, 200, img, GREEN);
	drawTree(550, 300, -90, 10, 10, img, YELLOW);
	writePPM("./Examples/Fractals/all_in_1.ppm", img);
	deletePPMImage(img);
	
	img = drawJulia(1920, 1080, 3);
	writePPM("./Examples/Fractals/Julia_set_z3.ppm", img);


	fprintf(stdout, "Image manipulation complete.\nProcess terminated successfully...");
	return 0;
}
