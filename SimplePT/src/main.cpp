
#include <iostream>
#include "Camera.h"
#include "PathTracer.h"


int main() {
	// cornell-box.xml
	//<light mtlname="Light" radiance="34.0, 24.0, 8.0"/>
	Position3 eye(278.0, 273.0, -800.0);
	Position3 lookat(278.0, 273.0, -799.0);
	Vector3 up(0.0, 1.0, 0.0);
	double fovy = 39.3077;
	//unsigned int width = 1024;
	unsigned int width = 2048;  // for debugging
	unsigned int height = 1024;

	Camera my_cam(eye, lookat, up, fovy, width, height);

	/* Load scene: TODO */
	//string filename;      // .obj file
	//Scene scn(filename, cam);  // load scene from file

	Scene my_scn;
	Position3 v0(549.6, 0, 559.2);
	Position3 v1(0, 0, 559.2);
	Position3 v2(0, 548.8, 559.2);
	std::shared_ptr<HittableBase> p_debug_triangle = std::make_shared<Mesh>(v0, v1, v2);
	my_scn.AddHittableObject(p_debug_triangle);

	PathTracer my_path_tracer(my_scn, my_cam);

	my_path_tracer.Render();

	return 0;
}