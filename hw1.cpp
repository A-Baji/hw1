/*******************************************************
 * Homework 1: Rasterization                           *
 * CS 148 (Summer 2016), Stanford University           *
 *-----------------------------------------------------*
 * Here you will implement the circle rasterization    *
 * method you derived in the written portion of the    *
 * homework.                                           *
 * To compile this in linux:                           *
 *        g++ hw1.cpp                                  *
 * Then, run the program as follows:                   *
 *        ./a.out 200                                  *
 * to generate a 200x200 image containing a circular   *
 * arc.  Note that the coordinate system we're using   *
 * places pixel centers at integer coordinates and     *
 * has the origin at the lower left.                   *
 * Your code will generate a .ppm file containing the  *
 * final image. These images can be viewed using       *
 * "display" in Linux or Irfanview in Mac/Windows.     *
 *******************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int size;
bool **image;

void renderPixel(int x, int y, int radius) {
	assert(x >= 0 && y >= 0 && x <= size && y <= size);
	image[x][y] = 1;
	image[y][x] = 1;

	// Draws the other half of semicircle of radius 100 for x >= 0
	if (radius == 100) {
		image[300-x][y] = 1; // Draws octant from 315 degrees to 360 degrees
		image[300-y][x] = 1; // Draws octant from 270 degrees to 315 degrees
	}
	
	// Draws the other half of semicircle of radius 150 for y >= 0
	if (radius == 150) {
		image[x][300-y] = 1; // Draws octant from 135 degrees to 180 degrees
		image[y][300-x] = 1; // Draws octant from 90 degree to 135 degrees
	}

}

void rasterizeArc(int radius) {
	// Code referenced from textbook, chapter 3.3.2, page 86
	int x = 0;
	int y = radius;
	int d = 1 - radius;
	
	// Offset by 150 to find center of minimum sized window of 300 pixels
	// This is necessary to fit the semicircle of radius 150
	renderPixel(x+150, y+150, radius); 

	while (y > x) {
		if (d < 0)
			d += 2*x+3;
		else {
			d += 2*(x-y)+5;
			y--;
		}
		x++;
		renderPixel(x+150, y+150, radius);
	}
}

// You shouldn't need to change anything below this point.

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " circleSize\n";
		return 0;
	}
	
#ifdef _WIN32
	sscanf_s(argv[1], "%d", &size);
#else
	sscanf(argv[1], "%d", &size);
#endif
	if (size < 300) { // Changed minimum size requirement
		std::cout << "Image size must be 300 or greater.\n";
		return 0;
	}
	
	// reserve image as 2d array
	image = new bool*[size+1];
	for (int i = 0; i <= size; i++) image[i] = new bool[size+1];
	
	// Calling rasterizeArc with 150 and 100 instead of size
	rasterizeArc(150);
	rasterizeArc(100);
	
	
	char filename[50];
#ifdef _WIN32
	sprintf_s(filename, 50, "circle.ppm");
#else
	sprintf(filename, "circle.ppm"); // Changed file name
#endif
	
	std::ofstream outfile(filename);
	outfile << "P3\n# " << filename << "\n";
	outfile << size+1 << ' ' << size+1 << ' ' << 1 << std::endl;

	for (int i = 0; i <= size; i++)
	for (int j = 0; j <= size; j++)	
		outfile << image[size-i][j] << " 0 0\n";
	
	// delete image data
	for (int i = 0; i <= size; i++) delete [] image[i];
	delete [] image;
	
	return 0;
}
