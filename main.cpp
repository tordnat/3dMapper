#include <sl/Camera.hpp>


int main(int argc, char **argv){
	
	sl::Camera zed;
	
	sl::InitParameters init_parameters;
	init_parameters.sdk_verbose = true;
	init_parameters.coordinate_units = sl::UNIT::METER;
	init_parameters.camera_resolution = sl::RESOLUTION::HD720;

	sl::SpatialMappingParameters mapping_parameters;
	mapping_parameters.resolution_meter = 0.03; // range [0.02-0.08], high to low qual
	mapping_parameters.range_meter = 4; // range [3.5 - 10]
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
	zed.enableSpatialMapping(mapping_parameters);

	sl::Mesh mesh;
	int timer=0;
	int update_period = init_parameters.camera_fps*60; 
	std::cout << "INITIALIZING";


	
	while (1){
		if(zed.grab() == sl::ERROR_CODE::SUCCESS){
			if (timer%update_period == 0 && zed.getSpatialMappingState() == sl::SPATIAL_MAPPING_STATE::OK){ 
				std::cout << "Requesting map: State: " << zed.getSpatialMappingState() << std::endl;
				zed.requestSpatialMapAsync(); //who likes threads anyways >:^)
			}

			if (zed.getSpatialMapRequestStatusAsync() == sl::ERROR_CODE::SUCCESS && timer > 0){

				std::cout << "Retrieving map: State: " << zed.getSpatialMappingState() << std::endl;
				zed.retrieveSpatialMapAsync(mesh);
				zed.extractWholeSpatialMap(mesh);
				mesh.filter(filter_parameters);
				mesh.applyTexture();
				mesh.save("mesh.obj");
			}

		timer++;
		}
	}

	

	


	zed.close();
	return EXIT_SUCCESS;

}
