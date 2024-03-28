
#include <iostream>
#include "Camera.h"
#include "PathTracer.h"


int main() {
	// cornell-box.xml
	std::string light_mtlname = "Light";
	Vector3 radiance(34.0, 24.0, 8.0);
	Position3 eye(278.0, 273.0, -800.0);
	Position3 lookat(278.0, 273.0, -799.0);
	Vector3 up(0.0, 1.0, 0.0);
	double fovy = 39.3077;
	//unsigned int width = 1024;
	//unsigned int height = 1024;
	unsigned int width = 512;
	unsigned int height = 512;

	//unsigned int width = 102;
	//unsigned int height = 102;
	//unsigned int width = 10;
	//unsigned int height = 10;


	//// veach-mis.xml
	//Position3 eye(28.2792, 5.2, 1.23612e-06);
	//Position3 lookat(0.0, 2.8, 0.0);
	//Vector3 up(0.0, 1.0, 0.0);
	//double fovy = 20.1143;
	////unsigned int width = 1280;
	////unsigned int height = 720;
	//unsigned int width = 128;
	//unsigned int height = 72;

	Camera my_cam(eye, lookat, up, fovy, width, height);

	/* Load scene: TODO */
	std::string filename = "./example-scenes-cg23/cornell-box/cornell-box.obj"; // .obj file
	std::string mtl_basepath = "./example-scenes-cg23/cornell-box/"; // .obj file
	//std::string filename = "./example-scenes-cg23/bathroom/bathroom.obj"; // .obj file
	//std::string mtl_basepath = "./example-scenes-cg23/bathroom/"; // .obj file
	//std::string filename = "./example-scenes-cg23/veach-mis/veach-mis.obj"; // .obj file
	//std::string mtl_basepath = "./example-scenes-cg23/veach-mis/"; // .obj file
	std::shared_ptr<HittableBase> p_mesh = std::make_shared<Mesh>(filename, mtl_basepath, light_mtlname, radiance);

	Scene my_scn;
	my_scn.AddHittableObject(p_mesh);

	/* one triangle */
	//Position3 v0(549.6, 0, 559.2);
	//Position3 v1(0, 0, 559.2);
	//Position3 v2(0, 548.8, 559.2);
	//std::shared_ptr<HittableBase> p_debug_triangle = std::make_shared<Mesh>(v0, v1, v2);
	//my_scn.AddHittableObject(p_debug_triangle);

	//my_scen.BuildBVH(); // todo
	PathTracer my_path_tracer(my_scn, my_cam);

	my_path_tracer.Render(10);

	return 0;
}