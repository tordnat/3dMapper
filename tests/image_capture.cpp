#include <sl/Camera.hpp>

using namespace sl;

int main(int argc, char **argv){
	
	// Camera object using zed sdk
	Camera zed;
	
	// Config initial params
	InitParameters init_params;
	init_params.camera_resolution = RESOLUTION::HD1080;
	init_params.camera_fps = 30;

	//open cam
	ERROR_CODE returned_state  = zed.open(init_params);
	if (returned_state != ERROR_CODE::SUCCESS){
		std::cout << "Error " << returned_state << "exit program. \n";
		return EXIT_FAILURE;
		}
	
	//get info 
	
	int zed_serial = zed.getCameraInformation().serial_number;
	printf("Hello! This is my serial number: %d\n", zed_serial);
	
	//Capture 50 images
	int i = 0;
	sl::Mat image;
	while (i < 50) {
		//grab image
		returned_state = zed.grab();
		//check return
		if (returned_state == ERROR_CODE::SUCCESS){
			//get images
			zed.retrieveImage(image, VIEW::LEFT);

			//get image info
			std::cout << "Image resolution: " << image.getWidth() << "x" 
				<< image.getHeight() << " || Image timestamp: " << image.timestamp.data_ns << std::endl;
		       i++;	
		}
	}


	//closing camera
	zed.close();
	return EXIT_SUCCESS;

}
