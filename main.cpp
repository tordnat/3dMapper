#include <sl/Camera.hpp>


int main(int argc, char **argv){
	
	sl::Camera zed;
	
	sl::InitParameters init_parameters;
	init_parameters.sdk_verbose = true;
	init_parameters.coordinate_units = sl::UNIT::METER;
	init_parameters.camera_resolution = sl::RESOLUTION::HD720;
	init_parameters.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP; // Use a right-handed Y-up coordinate system
	
	sl::SpatialMappingParameters mapping_parameters;
	mapping_parameters.resolution_meter = 0.01; // range [0.02-0.08], high to low qual
	mapping_parameters.range_meter = 1; // range [3.5 - 10]
	mapping_parameters.map_type = sl::SpatialMappingParameters::SPATIAL_MAP_TYPE::MESH;
	mapping_parameters.save_texture = true;

	sl::MeshFilterParameters filter_parameters;
	filter_parameters.set(sl::MeshFilterParameters::MESH_FILTER::MEDIUM);

	sl::ERROR_CODE returned_state  = zed.open(init_parameters);
	if (returned_state != sl::ERROR_CODE::SUCCESS){
		std::cout << "Error " << returned_state << "exit program. \n";
		return EXIT_FAILURE;
	}
	
	int zed_serial = zed.getCameraInformation().serial_number;
	printf("Successfully initialized ZED: %d\n", zed_serial);

	returned_state = zed.enablePositionalTracking();
	if (returned_state != sl::ERROR_CODE::SUCCESS){
		std::cout << "Error " << returned_state << "exit program. \n";
		return EXIT_FAILURE;
	}
	returned_state = zed.enableSpatialMapping(mapping_parameters);
	if (returned_state != sl::ERROR_CODE::SUCCESS){
		std::cout << "Error " << returned_state << "exit program. \n";
		return EXIT_FAILURE;
	}

	sl::Mesh mesh;
	int timer=0;
	int capture_duration = 120; 
	std::cout << "INITIALIZING" << std::endl;


	
	while (timer < capture_duration){
		if(zed.grab() == sl::ERROR_CODE::SUCCESS){
			timer++;
		}
	}
	
	zed.extractWholeSpatialMap(mesh);
	mesh.filter(filter_parameters);
	mesh.applyTexture();
	mesh.save("thumbsup.obj");
	
	zed.disableSpatialMapping();
	zed.disablePositionalTracking();
	zed.close();
	return EXIT_SUCCESS;

}
