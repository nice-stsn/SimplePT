
#include <iostream>
#include "Light.h"
#include "Camera.h"
#include "PathTracer.h"
#include "XmlLoader.h"


int main() {
	/* Select one example */
	//std::string example_name = "bathroom";
	std::string example_name = "cornell-box";
	//std::string example_name = "veach-mis";

	std::string resources_folder = "./example-scenes-cg23/";
	std::string example_folder = resources_folder  + example_name + '/';
	std::string mtl_basepath = example_folder; 
	std::string obj_filename = example_folder + example_name + ".obj";
	std::string xml_filename = example_folder + example_name + ".xml"; 

	/* load from xml file */
	std::vector<LightInfo> lights_info;
	Position3 eye;
	Position3 lookat;
	Vector3 up;
	double fovy = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	loadXmlFile(xml_filename, lights_info, eye, lookat, up, fovy, width, height);

	/* set camera */
	Camera my_cam(eye, lookat, up, fovy, width, height);

	/* set scene */
	std::shared_ptr<HittableBase> p_mesh = std::make_shared<Mesh>(obj_filename, mtl_basepath, lights_info);
	Scene my_scn;
	my_scn.AddHittableObject(p_mesh);

	/* set path tracer */
	PathTracer my_path_tracer(my_scn, my_cam);
	my_path_tracer.Render(10);

	return 0;
}