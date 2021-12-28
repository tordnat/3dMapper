#include <sl/Camera.hpp>

using namespace sl;

int main(int argc, char **argv){
	
	// Camera object using zed sdk
	Camera zed;
	
	// Config initial params
	InitParameters init_params;
	init_params.sdk_verbose = true;
	init_params.depth_mode = DEPTH_MODE::PERFORMANCE; 
	init_params.coordinate_units = UNIT::MILLIMETER;
	init_params.camera_resolution = RESOLUTION::HD1080;
	init_params.camera_fps = 30;
	
	//config runtime params
	RuntimeParameters runtime_params;
	runtime_params.sensing_mode = SENSING_MODE::STANDARD;

	//open cam
	ERROR_CODE returned_state  = zed.open(init_params);
	if (returned_state != ERROR_CODE::SUCCESS){
		std::cout << "Error " << returned_state << "exit program. \n";
		return EXIT_FAILURE;
		}
	
	//get info 
	
	int zed_serial = zed.getCameraInformation().serial_number;
	printf("Successfully initialized ZED: %d\n", zed_serial);
	
	//Capture 50 images, three formats
	int i = 0;
	sl::Mat image, depth, point_cloud;
	while (i < 50) {
		//grab image
		returned_state = zed.grab(runtime_params);
		//check return
		if (returned_state == ERROR_CODE::SUCCESS){
			//get images
			zed.retrieveImage(image, VIEW::LEFT);
			zed.retrieveMeasure(depth, MEASURE::DEPTH);
			zed.retrieveMeasure(point_cloud, MEASURE::XYZRGBA);
			
			//get and print image center
			int x = image.getWidth()/2;
			int y = image.getHeight()/2;
			sl::float4 point_cloud_value;
			point_cloud.getValue(x,y,&point_cloud_value);

	                if(std::isfinite(point_cloud_value.z)){
                        	float distance = sqrt(point_cloud_value.x * point_cloud_value.x + point_cloud_value.y * point_cloud_value.y + point_cloud_value.z * point_cloud_value.z);
				std::cout<<"Distance to Camera at {"<<x<<";"<<y<<"}: "<<distance<<"mm"<<std::endl;
            		}else std::cout<<"The Distance can not be computed at {"<<x<<";"<<y<<"}"<<std::endl;

			i++;	
		}
	}


	//closing camera
	zed.close();
	return EXIT_SUCCESS;

}
