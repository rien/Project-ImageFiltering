#ifndef H_FILTERMANAGER
#define H_FILTERMANAGER

#include "glew.h"
#include "GLFW/glfw3.h"

class Image;
class ImageFilter;

#include "EnumPreviewType.h"
#include "StructImagePreviewState.h"

#include <string> // For text strings
#include <vector> // For holding the states and buffers of an arbitrary number of images

// The filter manager contains all functions related to image loading and filtering
class FilterManager{

public:

	/****************************************************************/
	/* SINGLETON					                                */
	/****************************************************************/

	static FilterManager* instance; 
	static FilterManager* GetInstance();



private: 

	/****************************************************************/
	/* SETTINGS	& PREVIEW WINDOW                                    */
	/****************************************************************/

	int previewWindowAALevel;
	int previewWindowWidth;
	int previewWindowHeight;
	int previewWindowUpdateRate;

	GLFWwindow* previewWindow;
	bool previewWindowRunning;

	std::string vertexShaderFile;
	std::string fragmentShaderFile;



	/****************************************************************/
	/* OPENGL BUFFERS                                               */
	/****************************************************************/

	GLuint shaderProgram;
	std::vector<GLuint> textureBuffers; // Buffers for the individual images
	GLuint quadBuffer; // Buffer for the quads to render images on



	/****************************************************************/
	/* IMAGE PREVIEW STATE                                          */
	/****************************************************************/

	PreviewType activePreviewType;
	float scale;
	float mouseXPrevious;
	float mouseYPrevious;

	float snapgridIntervalX; // X axis intervals to which image previews should snap when grid-dragging
	float snapgridIntervalY; // Y axis intervals to which image previews should snap when grid-dragging

	float comparisonPercentage; // Percentage of the filtered image to show on top of the original image

	int selectedPreviewImage;
	std::vector<ImagePreviewState> imagePreviewStates;



	/****************************************************************/
	/* INITIALIZATION AND TERMINATION                               */
	/****************************************************************/

	// Initializes the filter manager
	bool Initialize();

	// Initializes GLFW
	bool InitializeGLFW();

	// Initializes GLEW
	bool InitializeGLEW();

	// Initializes FreeImage
	bool InitializeFreeImage();

	// Initializes the shader program
	bool InitializeShaderProgram();

	// Helper function - Loads and compiles an individual shader
	void LoadAndCompileShader(GLuint shaderID, std::string fileShader);

	void LoadAndCompileShader_DefaultVertexShader(GLuint shaderID);
	void LoadAndCompileShader_DefaultFragmentShader(GLuint shaderID);

	// Initializes OpenGL buffers for image previewing
	bool InitializeBuffers();

	// Registers input callbacks
	bool InitializeInputCallbacks();

	// Terminates the filter manager
	bool Terminate();

	// Terminates GLFW
	bool TerminateGLFW();

	// Terminates GLEW
	bool TerminateGLEW();

	// Terminates FreeImage
	bool TerminateFreeImage();

	// Frees the shader program
	bool TerminateShaderProgram();

	// Frees OpenGL buffers for image previewing
	bool TerminateBuffers();



	/****************************************************************/
	/* IMAGE MANAGEMENT                                             */
	/****************************************************************/

	// Rendering loop for the preview window
	void DisplayImageLoop(PreviewType previewType);

	// Updates the state of the preview window
	void DisplayImageLoop_Update(PreviewType previewType);

	// Draws the contents of the preview window
	void DisplayImageLoop_Draw(PreviewType previewType);

	// Stops the rendering loop for the preview window
	void DisplayImageLoop_Stop();
	
	// Display loop for "MultipleImages" preview mode. Draws multiple draggable images
	void DisplayImageLoop_MultipleImages();

	// Display loop for "ImageComparison" preview mode. Draws two images on top of eachother and allows blending between the two
	void DisplayImageLoop_ImageComparison();

	// Adds the image to the list of images to display
	void DisplayList_AddImage(Image* image);

	// Clears the list of images to display
	void DisplayList_Clear();

	// Sends an image to a buffer for the preview window
	void SendImageToBuffer(Image* image, GLuint buffer);

	// Moves a single image preview
	void ImagePreviewMove_Single(int imagePreviewID, float deltaX, float deltaY);

	// Moves all image previews
	void ImagePreviewMove_All(float deltaX, float deltaY);



	/****************************************************************/
	/* INPUT CALLBACK FUNCTIONS                                     */
	/****************************************************************/

	// Handles keyboard events
	static void GLFWKeyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);

	// Handles mouse button events
	static void GLFWMouseButtonEventCallback(GLFWwindow* window, int button, int action, int modifiers);

	// Handles mouse position events
	static void GLFWMousePositionEventCallback(GLFWwindow* window, double xPos, double yPos);

	// Handles mouse position events
	static void GLFWMouseScrollEventCallback(GLFWwindow* window, double xOffset, double yOffset);



public:

	/****************************************************************/
	/* CONSTRUCTORS AND DESTRUCTORS                                 */
	/****************************************************************/

	// Default constructor
	FilterManager();

	// Default destructor
	~FilterManager();

		

	/****************************************************************/
	/* IMAGE MANAGEMENT                                             */
	/****************************************************************/
	
	// Loads an image and returns it as an Image object
	Image* LoadImageFromFile(std::string filename);

	// Saves an Image object to a bitmap file
	void SaveImageToFile(Image* image, std::string filename);

	// Displays an image onscreen
	void DisplaySingleImage(Image* image);

	// Displays multiple images onscreen
	void DisplayMultipleImages(std::vector<Image*> images);

	// Displays two images simultaneously for making comparisons
	void DisplayImageComparison(Image* imageOne, Image* imageTwo);



	/****************************************************************/
	/* FILTER MANAGEMENT                                            */
	/****************************************************************/

	// Applies the filter defined in <filter> to the two source images <sourceImageOne> and <sourceImageTwo>. Returns the resulting image as an Image object.
	Image* ExecuteFilter(ImageFilter* filter, Image* sourceImageOne, Image* sourceImageTwo = 0);

};

#endif