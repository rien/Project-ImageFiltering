#include "FilterManager.h"
#include "Image.h"
#include "ImageFilter.h"
#include <iostream> // For printing information to the console
#include <windows.h>


/****************************************************************/
/* INCLUDES FOR THE FILTERS YOU WANT TO APPLY                   */
/****************************************************************/

 #include "ImageFilter_Desaturate_CPU.h"
 #include "ImageFilter_Desaturate_GPU.h"
 #include "ImageFilter_Invert_CPU.h"
 #include "ImageFilter_Invert_GPU.h"
 #include "ImageFilter_GreenScreen_CPU.h"
 #include "ImageFilter_GreenScreen_GPU.h"
 #include "ImageFilter_GaussianBlurFilter_GPU.h"
#include "ImageFilter_GaussianBlurFilter_CPU.h"
#include "ImageFilter_Sharpen_CPU.h"
#include "ImageFilter_Sharpen_GPU.h"
#include "ImageFilter_EdgeDetect_GPU.h"
#include "ImageFilter_EdgeDetect_CPU.h"
#include "ImageFilter_Median_CPU.h"
#include "ImageFilter_Median_GPU.h"
#include "ImageFilter_HueShift_GPU.h"
#include "ImageFilter_HistogramEqualization_GPU.h"
#include "ImageFilter_HistogramEqualization_CPU.h"


/****************************************************************/
/* MAIN FUNCTION (APPLICATION ENTRY POINT)                      */
/****************************************************************/

// Image filter project main function
int main(int argc, const char* argv[])
{
	////////////////////////////////////////////////////////////////
	// SAMPLE CODE                                                //
	////////////////////////////////////////////////////////////////

	// Create a FilterManager object to perform image and filter operations
	FilterManager* filterManager = FilterManager::GetInstance();

	// Load the required images from files
	Image* sourceImageLandscape = filterManager->LoadImageFromFile("images/GrassLandscape.jpg");
	Image* sourceImageGreenScreen = filterManager->LoadImageFromFile("images/GreenScreen.jpg");
	Image* sourceLena = filterManager->LoadImageFromFile("images/Lena_SaltPepperNoise.gif");


	
	//Dry run, om "op te warmen"
	ImageFilter_Desaturate_GPU testFilter;
	testFilter.Filter(sourceImageLandscape, nullptr);

	// List of filters with their name
	std::vector <std::pair<ImageFilter*, std::string>*> filters;
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Desaturate_CPU(), "Desaturate CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Desaturate_GPU(), "Desaturate GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Invert_CPU(), "Invert CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Invert_GPU(), "Invert GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Greenscreen_CPU(), "Greenscreen CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Greenscreen_GPU(), "Greenscreen GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_GaussianBlurFilter_GPU(), "Gaussian blur GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_GaussianBlurFilter_CPU(), "Gaussian blur CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Sharpen_GPU, "Sharpen GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Sharpen_CPU(), "Sharpen CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_EdgeDetect_GPU, "EdgeDetect GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_EdgeDetect_CPU, "EdgeDetect CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Median_GPU, "Median GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_Median_CPU, "Median CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_HueShift_GPU(), "HueShift GPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_HistogramEqualization_CPU(), "Histogran CPU"));
	filters.push_back(new std::pair<ImageFilter*, std::string>(new ImageFilter_HistogramEqualization_GPU(), "Histogran GPU"));


	Image* destination_image = filterManager->ExecuteFilter(new ImageFilter_Greenscreen_GPU(), sourceImageLandscape);;
	
	//filterManager->DisplaySingleImage(sourceImageLandscape);

	for (auto pair : filters){
		std::cout << std::endl << "Filter: " << pair->second << std::endl;
		destination_image = filterManager->ExecuteFilter(pair->first, sourceImageLandscape);
		//filterManager->DisplaySingleImage(destination_image);
		//filterManager->DisplaySingleImage(sourceImageLandscape);

		filterManager->DisplayImageComparison(sourceImageLandscape, destination_image);
		delete destination_image;
		delete pair->first;
		delete pair;
	}




	// Free memory allocated for images
	delete sourceImageLandscape;
	delete sourceImageGreenScreen;

	SetForegroundWindow(GetConsoleWindow());
	getchar();
}