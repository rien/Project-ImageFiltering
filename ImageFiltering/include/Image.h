#ifndef H_IMAGE
#define H_IMAGE

// An image represents a bitmap image along with its metadata
class Image{

private: 
	unsigned char* pixelData;
	unsigned int width;
	unsigned int height;

public:

	/****************************************************************/
	/* CONSTRUCTORS AND DESTRUCTORS                                 */
	/****************************************************************/

	// Constructor (when pixelData is already available)
	Image(unsigned char* pixelData, unsigned int width, unsigned int height);
	// Constructor (when an empty array of pixelData needs to be reserved)
	Image(unsigned int width, unsigned int height);
	// Deconstructor
	~Image();



	/****************************************************************/
	/* ATTRIBUTE GETTERS AND SETTERS                                */
	/****************************************************************/
	
	// Gets a pointer to the pixel data of the image, stored in a character array. Pixel data is stored in an interleaved sequence of R (red), G (green), B (blue) and A (alpha) components. Every individual component occupies one character of the array.
	unsigned char* GetPixelData();
	// Gets the width of the image, i.e. the horizontal number of pixels.
	unsigned int GetWidth();
	// Gets the height of the image, i.e. the vertical number of pixels.
	unsigned int GetHeight();

};

#endif