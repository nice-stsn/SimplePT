
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
	const int width = 1024;
	const int height = 1024;

	Camera my_cam(eye, lookat, up, fovy, width, height);

	/* Load scene: TODO */
	//string filename;      // .obj file
	//Scene scn(filename, cam);  // load scene from file
	//string outputImageName;
	//scn.Render(outputImageName); // render image
	Scene my_scn;

	PathTracer my_path_tracer(my_scn, my_cam);

	my_path_tracer.Render();

	return 0;
}