#pragma once
#include "Forward_declaration.h"
#include <string>
#include <vector>


bool loadXmlFile(
	const std::string& filePath,
	std::vector<LightInfo>& lights_info,
	Position3& eye,
	Position3& lookat,
	Vector3& up,
	double& fovy,
	unsigned int& width,
	unsigned int& height);

