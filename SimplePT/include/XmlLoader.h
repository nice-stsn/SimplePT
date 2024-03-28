#pragma once
#include <string>
#include <vector>
#include "Vector.h"

struct LightInfo;
bool loadXmlFile(
	const std::string& filePath,
	std::vector<LightInfo>& lights_info,
	Position3& eye,
	Position3& lookat,
	Vector3& up,
	double& fovy,
	unsigned int& width,
	unsigned int& height);

