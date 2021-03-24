#include "Header/image_manipulation.h"
#include "Header/fractals.h"

int main() {
	fprintf(stdout,"Process has started.\nPlease wait patiently...\n");		// start program

	// image manipulation

	PPMImage *img = readPPM("./Examples/Image Manipulation/west_1.ppm");	// read input image
	invertColor(img);														// apply negative filter
	writePPM("./Examples/Image Manipulation/west_1_negative.ppm", img);		// write the new image
	deletePPMImage(img);													// delete image data from memory
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	weightedGrayscaleFilter(img);											// apply grayscale filter
	writePPM("./Examples/Image Manipulation/west_1_grayscale.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	sepiaFilter(img);														// apply sepia filter
	writePPM("./Examples/Image Manipulation/west_1_sepia.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustSaturation(img, 50);												// adjust the saturation of image to 50%
	writePPM("./Examples/Image Manipulation/west_1_saturated_50.ppm",img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	resizeImage(img, 1280, 760);											// resize the image to 1280x760
	writePPM("./Examples/Image Manipulation/west_1_resized_1280x760.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustHue(img, 135);													// adjust the hue of image to 135
	writePPM("./Examples/Image Manipulation/west_1_hue_135.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustBrightness(img, 50);												// adjust the brightness of image to 50%
	writePPM("./Examples/Image Manipulation/west_1_brightness_50.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustContrast(img, 50);												// adjust the contrast of image to 50%
	writePPM("./Examples/Image Manipulation/west_1_contrast_50.ppm", img);
	deletePPMImage(img);
	
	img = readPPM("./Examples/Image Manipulation/west_1.ppm");
	adjustBlur(img, 3);														// blur the image by a sigma factor of 3
	writePPM("./Examples/Image Manipulation/west_1_blurred_3.ppm", img);
	deletePPMImage(img);


	// fractal generation

	img = createPPMImage(750, 750, BLACK);
	drawKochCurve(-350, -350, 350, 350, 5, img, RED);						// draw a red Koch curve from (-350, -350) to (350, 350) with 5 iterations
	rotateImage(img, 2);
	drawMultipleCircles(0, 0, 300, 15, 20, img, BLUE);						// draw 15 blue multiple circles centered at (0, 0) with outermost radius 300
	writePPM("./Examples/Fractals/Koch_curve_and_15_circles.ppm", img);
	deletePPMImage(img);

	img = drawMandelbrotSet(1800, 1800);									// draw Mandelbrot set
	drawKochCurve(-850, -850, -250, -250, 5, img, BLUE);					// draw Koch curve
	drawCircleFractal_xyPlane(-550, 550, 300, img, PURPLE);					// draw a circle fractal spreading over xy plane
	drawSierpinskiUtil2(550, -550, 400, img, CYAN);
	drawSierpinski(550, -550, 200, img, GREEN);								// draw Sierpinski triangle
	drawTree(550, 300, -90, 10, 10, img, YELLOW);							// draw tree fractal
	writePPM("./Examples/Fractals/all_in_1.ppm", img);
	deletePPMImage(img);
	
	img = drawJulia(1500, 1000, 3);											// draw Julia set with a zoom factor of 3
	writePPM("./Examples/Fractals/Julia_set_z3.ppm", img);
	deletePPMImage(img);
	
	img = createPPMImage(800, 800, RED);
	drawBarnsleyFern(1000000, 75, img);										// draw Barnsley fern with a million iterations
	writePPM("./Examples/Fractals/fern_i1000000.ppm", img);
	deletePPMImage(img);
	
	img = createPPMImage(500, 500, YELLOW);
	drawYinYangFractal(0, 0, 200, 0.4, 4, BLACK, WHITE, img);				// draw a black and white yin yang fractal with 4 iterations
	writePPM("./Examples/Fractals/yinyang_r200_i4.ppm", img);
	deletePPMImage(img);


	fprintf(stdout, "Image manipulation complete.\nProcess terminated successfully...");	// end program
	return 0;
}
