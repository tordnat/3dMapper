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

	//closing camera
	zed.close();
	return EXIT_SUCCESS;

}
